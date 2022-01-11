package GameState;

import Coop.Client;
import Coop.Server;
import main.GamePanel;

import java.awt.*;
import java.awt.event.KeyEvent;
import java.util.ArrayList;

/**
 * Represents the switching and managing tool for the game's states.
 * @author timusfed
 * @author shirover
 */
public class GameStateManager {

    /**
     * Represents the array of all possible states of the game.
     */
    private final ArrayList<GameState> gameStates;
    /**
     * Represent the index of the current game's state.
     */
    private int currentState;

    /**
     * Example of the index of the game's state.
     * Mostly useful as a public static constant.
     */
    public static final int MENU_STATE = 0;
    public static final int LEVEL_STATE = 1;
    public static final int BASEMENT_STATE = 2;

    /**
     * Represents the only possible choice inside the settings.
     * The sex change of the player.
     */
    private String playerSex;

    /**
     * Represents the server.
     */
    private Server server;
    /**
     * Represents the client.
     */
    private Client client;

    /**
     * Represents the status of the server.
     * Mostly useful as a public static constant.
     */
    public static boolean SERVER_IS_ONLINE = false;
    /**
     * Represents the status of the client.
     * Mostly useful as a public static constant.
     */
    public static boolean IM_CONNECTED = false;

    /**
     * Creates the GameStateManager.
     * Initialise and fills the array of the game's states.
     * Sets the current state.
     */
    public GameStateManager() {
        //standard settings
        playerSex = "man";

        gameStates = new ArrayList<>();
        gameStates.add(new MenuState(this));
        gameStates.add(new LevelState(this, "level"));
        gameStates.add(new LevelState(this, "basement"));

        //set menu state as active
        currentState = MENU_STATE;
    }

    /**
     * Updates the current state if it is available.
     * Content update depends on a state.
     */
    public void update() {
        if (gameStates.get(currentState).isAvailable)
            gameStates.get(currentState).update();
    }

    /**
     * Draws the current state if it is available.
     * Content draw depends on a state.
     * @param g The Graphics class, used to draw.
     */
    public void draw(Graphics2D g) {
        if (gameStates.get(currentState).isAvailable)
            gameStates.get(currentState).draw(g);
    }

    /**
     * Initialise the key 'on press' event on the current state.
     * Or sends the key code to the server if client is connected.
     * @param keyEvent The key code of the pressed button.
     */
    public void keyPressed(KeyEvent keyEvent) {
        if (IM_CONNECTED && currentState != MENU_STATE)
            client.keyPressed_(keyEvent);
        else
            gameStates.get(currentState).keyPressed_(keyEvent.getKeyCode());
    }

    /**
     * Initialise the key 'on release' event on the current state.
     * Or sends the key code to the server if client is connected.
     * @param keyEvent The key code of the released button.
     */
    public void keyReleased(KeyEvent keyEvent) {
        if (IM_CONNECTED && currentState != MENU_STATE)
            client.keyReleased_(keyEvent);
        else
            gameStates.get(currentState).keyReleased_(keyEvent.getKeyCode());
    }

    /**
     * Sets the server.
     * @param server A server, which is needed to set.
     */
    public void setServer(Server server) {
        this.server = server;
    }

    /**
     * Sets the client.
     * @param client A server, which is needed to set.
     */
    public void setClient(Client client) {
        this.client = client;
    }

    /**
     * Sets the player sex.
     * @param playerSex A string, representing the preferred sex of the player's avatar in game.
     */
    public void setPlayerSex(String playerSex) {
        this.playerSex = playerSex;
    }

    /**
     * Sets the current state, also initialise it.
     * @param state The int, representing the index of the state.
     */
    public void setState(int state) {
        currentState = state;
        gameStates.get(currentState).init();
    }

    /**
     * Gets the server.
     * @return server The server, which is need to be grabbed.
     */
    public Server getServer() {
        return server;
    }

    /**
     * Gets the current state.
     * @return gameStates.get(currentState) The GameState object of the current state form the states array.
     */
    public GameState getCurrentState() {
        return gameStates.get(currentState);
    }

    /**
     * Gets the index of the current state.
     * @return currentState The int, representing index of the current state.
     */
    public int getCurrentStateCode() {
        return currentState;
    }

    /**
     * Gets player's sex.
     * @return playerSex String, representing the sex, chosen by the player.
     */
    public String getPlayerSex() {
        return playerSex;
    }

    /**
     * Gets the array of all of the game's states.
     * @return gameStates An array, representing the game's states.
     */
    public ArrayList<GameState> getGameStates() {
        return gameStates;
    }
}
