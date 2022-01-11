package GameState;


import Entity.Player;
import TileMap.TileMap;
import TileMap.BackgroundTile;

import java.awt.*;
import java.net.Socket;

/**
 * Represents the state of the game.
 * @author timusfed
 * @author shirover
 */
public abstract class GameState {

    /**
     * Represents the preferred size of one tile in the game.
     * Depends on developer's choice of texture usage.
     */
    public static final int TILE_SIZE = 120;

    /**
     * Represents the GameStateManager.
     */
    protected GameStateManager gsm;
    /**
     * Represents the TileMap.
     */
    protected TileMap tileMap;
    /**
     * Represents the BackgroundTile.
     */
    protected BackgroundTile bg;
    /**
     * Represents the availability of the current GameSate.
     */
    protected boolean isAvailable;

    /**
     * Initialise the current game state.
     * Content Initialisation depends on type.
     */
    public abstract void init();
    /**
     * Updates the current game state. (Depends on type)
     */
    public abstract void update();
    /**
     * Draws the state to the screen. (Draw content depends on type)
     * @param g The Graphics class, used to draw.
     */
    public abstract void draw(Graphics2D g);
    /**
     * Initialise the key 'on press' event.
     * Grabs the param from GameStateManager - GamePanel.
     * @param keyCode The key code of the pressed button.
     */
    public abstract void keyPressed_(int keyCode);
    /**
     * Initialise the key 'on released' event.
     * Grabs the param from GameStateManager - GamePanel.
     * @param keyCode The key code of the released button.
     */
    public abstract void keyReleased_(int keyCode);
    /**
     * Sets the player on a state with the param's one.
     * @param player The player, which is needed to transfer to another state.
     */
    public abstract void setPlayer(Player player);
    /**
     * Initialise the key 'on press' event by the client.
     * @param client The client socket, which pressed the button.
     * @param keyCode The key code of the pressed button.
     */
    public abstract void keyPressed_(Socket client, int keyCode);
    /**
     * Initialise the key 'on release' event by the client.
     * @param client The client socket, which released the button.
     * @param keyCode The key code of the released button.
     */
    public abstract void keyReleased_(Socket client, int keyCode);
}
