package cz.cvut.fel.agents.pdv.swim;
import cz.cvut.fel.agents.pdv.dsand.Message;
import cz.cvut.fel.agents.pdv.dsand.Pair;
import java.util.*;
import java.util.stream.Collectors;

public class ActStrategy {

    // maximalni zpozdeni zprav
    private final int maxDelayForMessages;
    private List<String> otherProcesses;
    private HashMap<String, Integer> ping_reqs;
    private HashMap<String, Integer> pings;
    private HashMap<String, HashSet<String>> requestList;
    private int time;

    public ActStrategy(int maxDelayForMessages, List<String> otherProcesses,
                       int timeToDetectKilledProcess, int upperBoundOnMessages) {
        this.maxDelayForMessages = maxDelayForMessages;
        this.otherProcesses = otherProcesses;
        this.pings = new HashMap<>();
        this.ping_reqs = new HashMap<>();
        this.requestList = new HashMap<>();
        this.time = 0;
    }

    public List<Pair<String, Message>> act(Queue<Message> inbox, String disseminationProcess) {

        List<Pair<String, Message>> outbox = new ArrayList<>();

        while (!inbox.isEmpty()) {
            Message msg = inbox.poll();

            if (msg instanceof Ping) {
                Ping pingMsg = (Ping) msg;
                Pair<String, Message> ackPair = new Pair<>(pingMsg.sender, new Ack(pingMsg.recipient));
                outbox.add(ackPair);
            }

            else if (msg instanceof PingReq) {
                PingReq pingReqMsg = (PingReq) msg;
                Pair<String, Message> pingReqPair = new Pair<>(pingReqMsg.getProcess_id(), new Ping(pingReqMsg.getProcess_id()));

                if (!(this.pings.containsKey(pingReqMsg.getProcess_id()) || this.ping_reqs.containsKey(pingReqMsg.getProcess_id()))) {
                    outbox.add(pingReqPair);
                }

                // if no one asked yet -> create new record
                if (!this.requestList.containsKey(pingReqMsg.getProcess_id())) {
                    this.requestList.put(pingReqMsg.getProcess_id(), new HashSet<>());
                }
                this.requestList.get(pingReqMsg.getProcess_id()).add(pingReqMsg.sender); // new whoAsked
            }

            else if (msg instanceof Ack) {
                Ack ackMsg = (Ack) msg;

                this.pings.remove(ackMsg.getProcess_id()); // ping done
                this.ping_reqs.remove(ackMsg.getProcess_id()); // request done

                // send for all, who asked for process in ack
                if (this.requestList.containsKey(ackMsg.getProcess_id())) {
                    for (String whoAsked : this.requestList.get(ackMsg.getProcess_id())) {
                        outbox.add(new Pair<>(whoAsked, ackMsg));
                    }
                    this.requestList.remove(ackMsg.getProcess_id());
                }
            }
            else if (msg instanceof DeadProcessMessage) {
                DeadProcessMessage deadMsg = (DeadProcessMessage) msg;
                this.otherProcesses.remove(deadMsg.getProcessID());
                this.requestList.remove(deadMsg.getProcessID());
                this.ping_reqs.remove(deadMsg.getProcessID());
                this.pings.remove(deadMsg.getProcessID());
            }
        }

        HashSet<String> removeAfter = new HashSet<>();
        for (Map.Entry<String, Integer> entry : this.pings.entrySet()) {

            // if process last response time < max delay => send pfd
            if (entry.getValue() < this.time - this.maxDelayForMessages * 2) {

                List<String> others = this.otherProcesses;
                Collections.shuffle(others);
                for (int i = 0; i < 8; i++) {
                    outbox.add(new Pair<>(others.get(i), new PingReq(entry.getKey())));
                }
                this.ping_reqs.put(entry.getKey(), this.time);
                removeAfter.add(entry.getKey());
            }
        }

        for (String p : removeAfter) {
            this.pings.remove(p);
        }
        removeAfter.clear();

        for (Map.Entry<String, Integer> entry: this.ping_reqs.entrySet()) {
            if (entry.getValue() < this.time - this.maxDelayForMessages * 4) {
                outbox.add(new Pair<>(disseminationProcess, new PFDMessage(entry.getKey())));
                removeAfter.add(entry.getKey());
            }
        }

        for (String p : removeAfter) {
            this.ping_reqs.remove(p);
        }
        removeAfter.clear();

        if (this.time % 2 == 0) {
            // find random neighbor
            String rand_neighbor = String.valueOf(new Random().nextInt(this.otherProcesses.size()));

            // if random neighbor == main process -> go again
            while (rand_neighbor.equals("0") || this.pings.containsKey(rand_neighbor) || this.ping_reqs.containsKey(rand_neighbor)) {
                rand_neighbor = String.valueOf(new Random().nextInt(this.otherProcesses.size()));
            }

            // put new ping with curr time
            pings.put(rand_neighbor, this.time); // ping with curr time
            outbox.add(new Pair<>(rand_neighbor, new Ping(rand_neighbor)));
        }

        this.time++;
        return outbox.stream().distinct().collect(Collectors.toList()); // remove duplicates
    }

}

class Ping extends Message {

    private final String process_id;

    public Ping (String id) {
        this.process_id = id;
    }

    public String getProcess_id() {
        return process_id;
    }
};

class PingReq extends Message {
    private final String process_id;

    public PingReq (String id) {
        this.process_id = id;
    }

    public String getProcess_id() {
        return process_id;
    }
};

class Ack extends Message {
    private final String process_id;

    public Ack (String id) {
        this.process_id = id;
    }

    public String getProcess_id() {
        return process_id;
    }
};