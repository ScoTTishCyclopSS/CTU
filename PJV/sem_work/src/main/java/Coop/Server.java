package Coop;

import GameState.GameStateManager;

import java.io.*;
import java.net.*;
import java.nio.file.Paths;
import java.util.*;
import java.util.logging.Level;
import java.util.logging.Logger;

import static java.nio.file.Files.readAllBytes;

/**
 * Represents the server, where will be the game stored and run.
 * @author timusfed
 */
public class Server extends Thread {

    private final Logger logger = Logger.getLogger(Server.class.getName());

    private final ArrayList<Socket> clientSockets;
    private final Map<Socket, ObjectOutputStream> socketOutMap;

    private GameStateManager gsm;

    /**
     * Creates new server.
     * Declares empty clients map and array.
     * Sets the GameStateManager.
     * @param gsm Represents the GameStateManager.
     */
    public Server(GameStateManager gsm) {
        clientSockets = new ArrayList<>();
        socketOutMap = new HashMap<>();
        this.gsm = gsm;
    }

    /**
     * Runs the server in loo, waiting for the new players, also creates for every one his one Thread.
     * Adds new players to the map and array.
     */
    public void run() {
        try {
            ServerSocket serverSocket = new ServerSocket(Client.PORT);
            logger.info("->Server has been started!");

            // running infinite loop for getting client requests
            while (true) {
                // socket object to receive incoming client requests
                Socket clientSocket = null;
                clientSocket = serverSocket.accept();

                // displaying that new client is connected to server, add them to the array of the sockets
                if (clientSocket != null) {
                    logger.info("-->New client connected");
                    clientSockets.add(clientSocket);
                    socketOutMap.put(clientSocket, new ObjectOutputStream(clientSocket.getOutputStream()));

                    // create a new thread object, run client separately
                    new Thread(new ClientHandler(clientSocket, this)).start();
                }
            }
        } catch (IOException e) {
            logger.log(Level.WARNING, "Got an error on a server side: ", e);
            System.err.println(Arrays.toString(e.getStackTrace()));
        }
    }

    /**
     * Gets the array of the clients.
     * Useful, when is needed to work with only sockets.
     * @return clientSockets An array, representing all client sockets.
     */
    public ArrayList<Socket> getClientSockets() {
        return clientSockets;
    }

    /**
     * Sends the files of the all players to the clients.
     * Skip the administrator of the server (has flag SERVER_IS_ONLINE as 'true').
     */
    public void sendUpdatedPlayers() {

        File[] files = new File("players_online/").listFiles();

        clientSockets.stream().skip(1).forEach(clientSocket -> {
            try {
                ObjectOutputStream out = socketOutMap.get(clientSocket);
                out.writeUTF("PLAYERS_UPDATE"); // command
                out.flush();
                out.writeInt(files.length);
                out.flush();

                for (File file : files) { // basically 2x
                    out.writeUTF(file.getName() + "->" + new String(readAllBytes(Paths.get(file.getAbsolutePath())))); // name
                    out.flush();
                }

            } catch (IOException e) {
                System.err.println(Arrays.toString(e.getStackTrace()));
            }
        });
    }

    /**
     * Gets the GameStateManager.
     * @return gsm The GameStateManager object.
     */
    public GameStateManager getGsm() {
        return gsm;
    }
}
