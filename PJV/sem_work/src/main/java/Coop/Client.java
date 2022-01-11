package Coop;

import java.awt.event.KeyEvent;
import java.io.*;
import java.net.Socket;
import java.util.Arrays;
import java.util.logging.Logger;

/**
 * Represents the client connection to the game server.
 * @author timusfed
 */
public class Client extends Thread {

    /**
     * Represents the port of the server.
     */
    public static int PORT = 1337;

    private final Logger logger = Logger.getLogger(Client.class.getName());

    private final String ip;

    private PrintWriter out;


    private ObjectInputStream inObj;

    /**
     * Creates new client without connection.
     * Sets the ip address.
     * @param ip String, representing IP address of the server.
     */
    public Client(String ip) {
        this.ip = ip;
    }

    /**
     * Run the client thread.
     * Creates new connection.
     * Sets new output and input object streams.
     * In loop waits the info from the server.
     */
    public void run() {
        try {
            Socket clientSocket = new Socket(ip, PORT);
            out = new PrintWriter(clientSocket.getOutputStream(), true);
            inObj = new ObjectInputStream(clientSocket.getInputStream());

            while (true) {
                String command = "";
                try { command = inObj.readUTF(); } catch (IOException ignored) { }
                if (command.equals("PLAYERS_UPDATE"))
                    updatedPlayersListener();
            }
        } catch (Exception e) {
            System.err.println(Arrays.toString(e.getStackTrace()));
        }
    }

    private void updatedPlayersListener() throws IOException {
        int playersCount = inObj.readInt();
        for(int i = 0; i < playersCount; i++) {
            String[] incomePl = inObj.readUTF().split("->");
            File file = new File("players_income/" + incomePl[0]);
            file.getParentFile().mkdirs();
            file.createNewFile();
            FileWriter out = new FileWriter(file);
            out.write(incomePl[1]);
            out.close();
        }
    }

    /**
     * Sends the key code of the pressed key to the server if client is connected.
     * @param keyEvent The key event.
     */
    public void keyPressed_(KeyEvent keyEvent) {
        out.println(keyEvent.paramString().split(",")[0] + ":" + keyEvent.getKeyCode());
    }

    /**
     * Sends the key code of the released key to the server if client is connected.
     * @param keyEvent The key event.
     */
    public void keyReleased_(KeyEvent keyEvent) {
        out.println(keyEvent.paramString().split(",")[0] + ":" + keyEvent.getKeyCode());
    }
}
