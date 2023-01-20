// author: @timusfed

package cz.cvut.fel.agents.pdv.exclusion;

import cz.cvut.fel.agents.pdv.clocked.ClockedMessage;
import cz.cvut.fel.agents.pdv.clocked.ClockedProcess;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class ExclusionPrimitive {

    enum AcquisitionState {
        RELEASED,
        WANTED,
        HELD
    }

    enum AccessLvl {
        CAN_ENTER,
        NOT_YET
    }

    private ClockedProcess owner;
    private String criticalSectionName;
    private String[] allAccessingProcesses;
    private AcquisitionState state;
    private HashMap<String, AccessLvl> okMessages; // have to use dict
    private List<Request> deferredRequests;
    private int myLastReqTime;

    public ExclusionPrimitive(ClockedProcess owner, String criticalSectionName, String[] allProcesses) {
        this.owner = owner;
        this.criticalSectionName = criticalSectionName;
        this.allAccessingProcesses = allProcesses;
        this.state = AcquisitionState.RELEASED;
        this.okMessages = new HashMap<>();
        this.deferredRequests = new ArrayList<>();
        this.myLastReqTime = 0;
    }

    public boolean accept(ClockedMessage m) {

        if (m instanceof Request) {
            Request r = (Request) m;
            int Tj = this.myLastReqTime;
            int Ti = r.sentOn;
            boolean goodTime = isWanted() && (Tj < Ti || (Tj == Ti && r.GetRecipientId() < r.GetSenderId()));

            if (r.GetSection().equals(this.criticalSectionName)) {
                if (isHeld() || goodTime) {
                    this.deferredRequests.add(r);
                } else {
                    this.owner.send(r.sender, new Ok(this.criticalSectionName));
                }
                return true;
            } else {
                return false;
            }
        }

        if (m instanceof Ok) {
            Ok ok = (Ok) m;
            if (this.criticalSectionName.equals(ok.GetSection())) {
                this.okMessages.put(ok.sender, AccessLvl.CAN_ENTER);
                if (accessGranted()) {
                    this.owner.increaseTime();
                    this.state = AcquisitionState.HELD;
                }
                return true;
            } else {
                return false;
            }
        }

        return false;
    }

    public void requestEnter() {
        this.owner.increaseTime();
        this.state = AcquisitionState.WANTED;
        this.myLastReqTime = this.owner.getTime();
        for (String p : this.allAccessingProcesses) {
            if (!p.equals(this.owner.id)) {
                this.okMessages.put(p, AccessLvl.NOT_YET);
                this.owner.send(p, new Request(this.criticalSectionName));
            }
        }
    }

    public void exit() {
        this.owner.increaseTime();
        this.state = AcquisitionState.RELEASED;
        for (Request r : this.deferredRequests) {
            this.owner.send(r.sender, new Ok(this.criticalSectionName));
        }
        this.deferredRequests.clear();
    }

    public String getName() {
        return criticalSectionName;
    }

    public boolean isHeld() {
        return state == AcquisitionState.HELD;
    }

    public boolean isWanted() {
        return state == AcquisitionState.WANTED;
    }

    private boolean accessGranted() {
        for (String p : this.allAccessingProcesses) {
            if (!okMessages.containsKey(p) && !p.equals(this.owner.id)) {
                return false;
            }
            if (okMessages.get(p) == AccessLvl.NOT_YET) {
                return false;
            }
        }
        return true;
    }
}


class Request extends ClockedMessage {

    private final String criticalSection;
    public Request(String criticalSection) {
        this.criticalSection = criticalSection;
    }

    public String GetSection() {
        return this.criticalSection;
    }

    public int GetSenderId() {
        return idFromString(this.sender);
    }

    public int GetRecipientId() {
        return idFromString(this.recipient);
    }

    private int idFromString(String processIdStr) {
        int id = 0;
        for (Character c : processIdStr.toCharArray()) {
            if (c >= '0' && c < '9') {
                id *= 10;
                id += Integer.parseInt(c.toString());
            }
        }
        return id;
    }
}

class Ok extends ClockedMessage {

    private final String criticalSection;

    public Ok(String criticalSection) {
        this.criticalSection = criticalSection;
    }

    public String GetSection() {
        return this.criticalSection;
    }

    public int GetSenderId() {
        return idFromString(this.sender);
    }

    public int GetRecipientId() {
        return idFromString(this.recipient);
    }

    private int idFromString(String processIdStr) {
        int id = 0;
        for (Character c : processIdStr.toCharArray()) {
            if (c >= '0' && c < '9') {
                id *= 10;
                id += Integer.parseInt(c.toString());
            }
        }
        return id;
    }

}