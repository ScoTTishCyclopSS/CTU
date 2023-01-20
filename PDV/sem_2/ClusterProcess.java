package cz.cvut.fel.agents.pdv.student;

import cz.cvut.fel.agents.pdv.dsand.Message;
import cz.cvut.fel.agents.pdv.dsand.Pair;
import cz.cvut.fel.agents.pdv.evaluation.StoreOperationEnums;
import cz.cvut.fel.agents.pdv.raft.RaftProcess;
import cz.cvut.fel.agents.pdv.raft.messages.*;

import java.io.Serializable;
import java.util.*;
import java.util.concurrent.ThreadLocalRandom;
import java.util.function.BiConsumer;
import java.util.stream.Collectors;

import static java.lang.Math.max;

public class ClusterProcess extends RaftProcess<Map<String, String>> {

    // ostatni procesy v clusteru
    private final List<String> otherProcessesInCluster;
    // maximalni spozdeni v siti
    private final int networkDelays;
    private String leader; // current leader
    private State state; // current state for server
    private Integer myTime; // local time (tick)
    private Integer electionTimer;
    private Set<String> myElectorate; // votes for me, set for being unique
    private Map<String, Integer> bpm; // to track "heart-beating" of others
    private Map<String, Integer> rpc; // to track "remote procedure calls" of others
    private Map<String, Integer> nextIndex; // next log entry index for each process to send
    private Map<String, Integer> matchIndex; // highest log entry index for each process
    private Log log;
    private Integer currentTerm; // latest term for this server
    private String myVote;
    private HashMap<String, String> data;
    private Integer currentCommit; // index of the highest log entry to commit

    public ClusterProcess(String id, Queue<Message> inbox, BiConsumer<String, Message> outbox,
                          List<String> otherProcessesInCluster, int networkDelays) {
        super(id, inbox, outbox);
        this.otherProcessesInCluster = otherProcessesInCluster;
        this.networkDelays = max(1, networkDelays);
        this.leader = null;
        this.myElectorate = new HashSet<>();
        this.bpm = new HashMap<>();
        this.rpc = new HashMap<>();
        this.log = new Log();
        this.myTime = 0;
        this.electionTimer = 0; // election timeout
        this.currentTerm = 0;
        this.myVote = null;
        this.state = State.Follower;

        resetDamnTimer(); // for the first election

        for (String p: otherProcessesInCluster) {
            bpm.put(p, 0);
            rpc.put(p, 0);
        }

        this.nextIndex = new HashMap<>();
        this.matchIndex = new HashMap<>();

        this.currentCommit = 0;

        this.data = new HashMap<>();
    }

    /**
     * Resents the election timer, random time.
     */
    void resetDamnTimer() {
        // ~150 to 300 ms
        Integer timeout = ThreadLocalRandom.current().nextInt(5 * this.networkDelays, 18 * this.networkDelays);
        electionTimer = myTime + timeout;
    }

    @Override
    public Optional<Map<String, String>> getLastSnapshotOfLog() {
        return Optional.of(data);
    }

    @Override
    public String getCurrentLeader() {
        return leader;
    }

    /**
     * RPC contains term as 't' > current term: current term = 't'.
     * Also, change state to follower.
     * @param term - term
     */
    private void stepDown(Integer term) {
        // stop being a leader
        state = State.Follower;
        // update term
        this.currentTerm = term;
        // reset election timer
        if (electionTimer == Integer.MAX_VALUE || electionTimer > myTime) {
            resetDamnTimer();
        }
    }

    @Override
    public void act() {

        this.myTime += 1;

        election();

        electionResult();

        commit();

        sendAppendEntryFunc(); // leader only

        for (Message m : inbox) {
            if (m instanceof RequestVote)
            {
                requestVoteCallback(m);
            }
            else if (m instanceof RequestVoteReply)
            {
                requestReplyCallback(m);
            }
            else if (m instanceof AppendEntry)
            {
                appendEntryCallback(m);
            }
            else if (m instanceof AppendEntryReply)
            {
                appendEntryReplyCallback(m);
            }
            else if (m instanceof ClientRequestWhoIsLeader)
            {
                ClientRequestWhoIsLeader c = (ClientRequestWhoIsLeader) m;
                send(c.sender, new ServerResponseLeader(c.getRequestId(), getCurrentLeader()));
            }
            else if (m instanceof ClientRequestWithContent)
            {
                messageWithContentCallback(m);
            }
        }

        inbox.clear();
    }

    /**
     * If current time of a node passed - the new election process should be started.
     */
    private void election() {

        if (myTime > electionTimer && !state.equals(State.Leader)) {
            // change my state to candidate
            state = State.Candidate;

            // clear data bpm
            bpm.clear();
            rpc.clear();
            myElectorate.clear();

            // vote for myself
            myVote = getId();
            myElectorate.add(getId());

            // increase term
            currentTerm += 1;

            // clear data log
            nextIndex.clear();
            matchIndex.clear();

            for (String p : otherProcessesInCluster) {
                bpm.put(p, 0);
                rpc.put(p, 0);

                nextIndex.put(p, 1);
                matchIndex.put(p, 0);
            }

            // send vote requests RPCs to all others
            for (String p : otherProcessesInCluster) {
                if(rpc.get(p) <= myTime) {
                    rpc.put(p, networkDelays * 3);
                    send(p, new RequestVote(currentTerm, log.size(), log.getLastTerm(), getId()));
                }
            }

            // also, don't forget to reset timer for further election!
            resetDamnTimer();
        }
    }

    /**
     * Handle a vote request type of message.
     * If the process hasn't voted yet
     * in this term then it votes for the candidate.
     * And the process resets its election timeout.
     * @param m - message
     */
    private void requestVoteCallback(Message m) {
        RequestVote requestVote = (RequestVote) m;

        if(currentTerm < requestVote.term) {
            stepDown(requestVote.term);
        }

        boolean sameTerm = currentTerm.equals(requestVote.term);
        boolean validVote = myVote == null || myVote.equals(requestVote.sender);
        boolean validLogTerm = requestVote.lastLogTerm > log.getLastTerm() ||
                (requestVote.lastLogTerm.equals(log.getLastTerm()) && requestVote.lastLogIndex >= log.size());

        boolean voteGranted  = false;
        if (sameTerm && validVote && validLogTerm) {
            // if all necessary conditions are  --> process votes for sender
            myVote = requestVote.sender;
            voteGranted = true;
            resetDamnTimer();
        }

        send(requestVote.candidateId, new RequestVoteReply(currentTerm, voteGranted));
    }

    /**
     * Handle a request vote reply type of message.
     * Process adds positive replies to its electorate.
     * @param m - message
     */
    private void requestReplyCallback(Message m) {
        RequestVoteReply requestVoteReply = (RequestVoteReply) m;

        if(currentTerm < requestVoteReply.term) {
            stepDown(requestVoteReply.term);
        }

        // must be checked in ONE time...
        if (state.equals(State.Candidate) && currentTerm.equals(requestVoteReply.term)) {
            if(requestVoteReply.voteGranted) {
                myElectorate.add(requestVoteReply.sender);
            }
            rpc.put(requestVoteReply.sender, Integer.MAX_VALUE);
        }
    }

    /**
     * Once the candidate has the majority of votes - it becomes a leader.
     */
    private void electionResult() {
        if (!state.equals(State.Candidate))
            return;

        //divide by 2 for majority
        if (otherProcessesInCluster.size() / 2 < myElectorate.size()) {
            leader = getId();
            state = State.Leader; // yay, I won! but what did it cost...?

            electionTimer = Integer.MAX_VALUE; // stop timer

            // clear data
            bpm.clear();
            rpc.clear();

            // clear data log
            nextIndex.clear();

            for (String p: otherProcessesInCluster) {
                bpm.put(p, 0);
                rpc.put(p, Integer.MAX_VALUE);
                nextIndex.put(p, log.size() + 1); // todo: test not 0
            }
        }
    }

    /**
     * After becoming the leader - process sends append entries to other processes.
     * These messages are sent in intervals specified by the heartbeat timeout.
     */
    private void sendAppendEntryFunc() {

        if (!state.equals(State.Leader))
            return;

        for (String p : otherProcessesInCluster) {
            boolean alive = bpm.get(p) <= myTime;
            boolean aliveAsRpc = (rpc.get(p) <= myTime) && (nextIndex.get(p) <= log.size());

            if (alive || aliveAsRpc) {
                Integer prevLogIndex = nextIndex.get(p) - 1;
                Integer prevLogTerm = log.getTermById(prevLogIndex);
                Integer lastLogIndex = Math.min(log.size(), prevLogIndex + 6);

                if(prevLogIndex > matchIndex.get(p)) {
                    lastLogIndex = prevLogIndex;
                }

                Integer commitToSend = currentCommit > lastLogIndex ? lastLogIndex : currentCommit;

                //send(p, new AppendEntry(currentTerm, getId()));
                //send(p, new AppendEntry(currentTerm, prevLogIndex, prevLogTerm, currentCommit, getId(), log.getEntries()));
                send(p, new AppendEntry(currentTerm, prevLogIndex, prevLogTerm, commitToSend, getId(),
                        new ArrayList<>(log.getEntries().subList(prevLogIndex, lastLogIndex)))
                );

                // change delay as waiting for the response
                bpm.put(p, myTime + networkDelays * 2 - 1);
                rpc.put(p, myTime + networkDelays);
            }
        }
    }

    /**
     * Responses to append entry type of message.
     * The process becomes follower.
     * @param m - message
     */
    private void appendEntryCallback(Message m) {
        AppendEntry appendEntry = (AppendEntry) m;
        boolean success = false;
        int logInsertIndex = 0;

        if(currentTerm < appendEntry.term) {
            stepDown(appendEntry.term);
        }

        if (currentTerm.equals(appendEntry.term)) {
            // become follower
            state = State.Follower;

            // designate leader
            leader = appendEntry.leaderId;

            // reset election timer
            resetDamnTimer();

            // does our log contain an entry at prevLogIndex whose term matches prevLogTerm
            boolean validTermByIndex = log.getTermById(appendEntry.prevLogIndex).equals(appendEntry.prevLogTerm)
                    && appendEntry.prevLogIndex <= log.size();

            if(appendEntry.prevLogIndex.equals(0) || validTermByIndex)
            {
                success  = true;

                logInsertIndex = appendEntry.prevLogIndex;

                for(LogEntry e: appendEntry.entries) {

                    // find an insertion point - where there's a term mismatch between
                    // the existing log starting at prevLogIndex + 1 and the new entries sent
                    // in the RPC.

                    if(!e.term.equals(log.getTermById(logInsertIndex + 1))) {
                        while(log.size() > logInsertIndex) {
                            log.pop();
                        }
                        log.push(e);
                    }
                    logInsertIndex += 1;
                }

                if (currentCommit < appendEntry.leaderCommit) {
                    for (LogEntry e: log.getEntries().subList(currentCommit, appendEntry.leaderCommit))
                    {
                        entryToData(e);
                        currentCommit += 1;
                    }
                }
            }
        }

        //send(appendEntry.sender, new AppendEntryReply(currentTerm, success));
        send(appendEntry.sender, new AppendEntryReply(currentTerm, success, logInsertIndex));
    }

    /**
     * Handles append entry reply type of message.
     */
    private void appendEntryReplyCallback(Message m) {
        AppendEntryReply appendEntryReply = (AppendEntryReply) m;
        Integer currHighestLogEntry = matchIndex.get(appendEntryReply.sender);

        if(currentTerm < appendEntryReply.term) {
            stepDown(appendEntryReply.term);
        }

        if (!state.equals(State.Leader))
            return;

        if(currentTerm.equals(appendEntryReply.term)) {

            // by standard
            rpc.put(appendEntryReply.sender, 0);
            nextIndex.put(appendEntryReply.sender,
                    Math.max(1, nextIndex.get(appendEntryReply.sender) - 1));

            // if the process agreed in appending with the new leader,
            // then we update his log entry
            if(appendEntryReply.success) {
                nextIndex.put(appendEntryReply.sender, appendEntryReply.index + 1);
                // also we need to update its highest log entry
                matchIndex.put(appendEntryReply.sender,
                        Math.max(currHighestLogEntry, appendEntryReply.index));
            }
        }
    }

    private void messageWithContentCallback(Message m) {

        ClientRequestWithContent c = (ClientRequestWithContent) m;

        if(!state.equals(State.Leader)) {
            send(c.sender, new ServerResponseLeader(c.getRequestId(), getCurrentLeader()));
            return;
        }

        Pair<String, String> content = (Pair<String, String>) c.getContent();
        StoreOperationEnums op = (StoreOperationEnums) c.getOperation();

        if (op == StoreOperationEnums.GET)
        {
            send(c.sender, new ServerResponseWithContent<>(c.getRequestId(),
                    data.get(content.getFirst())));
        }
        else
        {
            log.append(c.getRequestId(), new LogEntry(currentTerm, new Request(op, content.getFirst(),
                    content.getSecond(), c.getRequestId(), c.sender)));
        }
    }

    private void commit() {
        if (!state.equals(State.Leader))
            return;

        ArrayList<Integer> matchedIds = new ArrayList<>(matchIndex.values());
        matchedIds.sort(Collections.reverseOrder());

        Integer id = 0;
        for (int i = 0; i < otherProcessesInCluster.size(); i++)
        {
            if(log.getTermById(i).equals(currentTerm)){
                id = matchedIds.get(i);
                if(i > 0) {
                    id = matchedIds.get(i - 1);
                }
                break;
            }
        }

        if(id.equals(otherProcessesInCluster.size()))
            return;

        if (currentCommit < id)
        {
            for (LogEntry e: log.getEntries().subList(currentCommit, id)) {

                entryToData(e);
                send(e.rpc.client, new ServerResponseConfirm(e.rpc.requestId));
            }
            currentCommit = id;
        }
    }

    private void entryToData(LogEntry e) {
        Request r = e.rpc;
        String k = r.key;
        String v = r.value;

        if(r.op.equals(StoreOperationEnums.APPEND))
        {
            if(!data.containsKey(k))
                data.put(k, v);
            else
                data.put(k, data.get(k) + v);
        }
        else if (r.op.equals(StoreOperationEnums.PUT))
        {
            data.put(k, v);
        }
    }
}

class Log {

    private List<LogEntry> entries;

    public Log() {
        this.entries = new ArrayList<>();

    }

    public void append(String requestId, LogEntry e) {
        // if it was not requested already --> add new request into log
        Set<String> keys = entries.stream().map(k -> k.rpc.requestId).collect(Collectors.toSet());
        if (!keys.contains(requestId)) {
            entries.add(e);
        }
    }

    Integer size() {
        return entries.size();
    }

    public Integer getTermById(Integer id) {
        if( id < 1 || id > entries.size()) {
            return 0;
        }
        return entries.get(id - 1).term;
    }

    public List<LogEntry> getEntries() {
        return entries;
    }

    public void push(LogEntry e) {
        entries.add(e);
    }

    public void pop() {
        entries.remove(size() - 1); // pop from entries
    }

    public Integer getLastTerm() {
        if(entries.size() > 0) {
            return entries.get(entries.size() - 1).term;
        }
        return 0;
    }
}

class Request implements Serializable {
    String key, value, requestId, client;
    StoreOperationEnums op;

    public Request(
            StoreOperationEnums op,
            String key,
            String value,
            String requestId,
            String client
    ) {
        this.op = op;
        this.key = key;
        this.value = value;
        this.requestId = requestId;
        this.client = client;
    }
}

class LogEntry implements Serializable {
    public Integer term; // term at which the client command was sent
    public Request rpc; // client command

    public LogEntry(Integer term, Request rpc) {
        this.term = term;
        this.rpc = rpc;
    }
}

class AppendEntryReply extends Message {
    public Integer term;
    public boolean success;
    public Integer index;

    AppendEntryReply(Integer term, boolean success, int index) {
        this.term = term;
        this.success = success;
        this.index = index;
    }

    public AppendEntryReply(Integer term, boolean success) {
        this.term = term;
        this.success = success;
    }
}

class AppendEntry extends Message {
    public Integer term; // current term of the leader
    public List<LogEntry> entries; // new data to sync
    public Integer prevLogTerm; // metadata for log correctness
    public Integer prevLogIndex; // metadata for log correctness
    public Integer leaderCommit; // what index have been received by the majority
    public String leaderId;

    public AppendEntry(
            Integer term,
            Integer prevLogIndex,
            Integer prevLogTerm,
            Integer leaderCommit,
            String leaderId,
            List<LogEntry> entries
    ) {
        this.term = term;
        this.prevLogIndex = prevLogIndex;
        this.prevLogTerm = prevLogTerm;
        this.leaderCommit = leaderCommit;
        this.leaderId = leaderId;
        this.entries = entries;
    }

    public AppendEntry(Integer term, String leaderId) {
        this.term = term;
        this.leaderId = leaderId;
    }
}

class RequestVoteReply extends Message {
    public Integer term;
    public boolean voteGranted;

    public RequestVoteReply(Integer term, boolean chosen) {
        this.term = term;
        this.voteGranted = chosen;
    }
}

class RequestVote extends Message {
    public Integer term, lastLogIndex, lastLogTerm;
    public String candidateId;

    public RequestVote(
            Integer term,
            Integer lastLogIndex,
            Integer lastLogTerm,
            String candidateId
    ) {
        this.term = term;
        this.lastLogIndex = lastLogIndex;
        this.lastLogTerm = lastLogTerm;
        this.candidateId = candidateId;
    }
}

enum State implements IOperation {
    Leader,
    Candidate,
    Follower;

    @Override
    public int getOrder() {
        return 0;
    }

    @Override
    public String getName() {
        return null;
    }
}