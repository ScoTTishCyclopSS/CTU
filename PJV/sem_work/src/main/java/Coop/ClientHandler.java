package Coop;

import java.io.*;
import java.net.Socket;
import java.util.Arrays;

/**
 * Represents the handler for the single client on the server side.
 * The part of the server.
 * @author timusfed
 */
public class ClientHandler implements Runnable {

    private final Socket clientSocket;
    private final Server server;

    private PrintWriter out;
    private BufferedReader in;

    /**
     * Creates the new thread, which will handle the client.
     * Sets the server and client socket.
     * @param clientSocket Represents the client socket.
     * @param server Represents the server, where the client has been connected.
     */
    public ClientHandler(Socket clientSocket, Server server) {
        this.clientSocket = clientSocket; // client, which holds this handler
        this.server = server;
    }

    /**
     * Runs the handler in loop, waiting for the activity from the client side.
     * Basically waits for the key event to happen.
     */
    @Override
    public void run() {
        try {
            out = new PrintWriter(clientSocket.getOutputStream(), true);
            in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
            while (true) {
                keyReader();
            }
        } catch (Exception e) {
            System.err.println(Arrays.toString(e.getStackTrace()));
        }
    }

    private void keyReader() throws Exception {
        // just tell that it's not empty, but it doesn't tell you what is inside
        if (in.ready()) {
            String income = in.readLine();
            //for the test
            if (income.equals("ping"))
                out.write("pong");
            else {
                String[] incomeKey = income.split(":");
                if (incomeKey[0].equals("KEY_PRESSED"))
                    server.getGsm().getCurrentState().keyPressed_(clientSocket, Integer.parseInt(incomeKey[1]));
                else
                    server.getGsm().getCurrentState().keyReleased_(clientSocket, Integer.parseInt(incomeKey[1]));
            }
        }
    }
}
