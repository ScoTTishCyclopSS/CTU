package GameState;

import Entity.Player;
import TileMap.TileMap;
import TileMap.BackgroundTile;
import org.json.simple.parser.ParseException;

import java.awt.*;
import java.awt.event.KeyEvent;
import java.io.File;
import java.io.IOException;
import java.net.Socket;
import java.util.*;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.logging.Logger;

/**
 * Represents the playable state of the game on the CO-OP mode.
 * @author timusfed
 */
public class LevelStateCoop extends GameState{

    Logger logger = Logger.getLogger(LevelStateCoop.class.getName());
    private final GameStateManager gsm;
    private final Map<Socket, Player> socketPlayerMap;
    private final ArrayList<Player> players;

    /**
     * Creates the new level object.
     * Initialise the Map with sockets and their players, simple player array (for the client side)
     * Sets the GameStateManager, Availability.
     * @param gsm Represents the GameStateManager.
     */
    public LevelStateCoop(GameStateManager gsm) {
        this.gsm = gsm;
        socketPlayerMap = new HashMap<>();
        players = new ArrayList<>();
        isAvailable = false;
    }

    @Override
    public void init() {
        tileMap = new TileMap(TILE_SIZE);
        tileMap.loadTiles("/testmap/level/tileset.gif");
        tileMap.loadMap("/testmap/level/map.map");
        tileMap.setTween(0.07);
        tileMap.setPosition(0, 0);

        bg = new BackgroundTile("/Backgrounds/level/test.png", 0.1);

        if (GameStateManager.SERVER_IS_ONLINE) {
            logger.info("players connected: " + gsm.getServer().getClientSockets().size());

            gsm.getServer().getClientSockets().forEach(clientSocket -> {
                try { socketPlayerMap.put(clientSocket, new Player(tileMap, "man", null)); }
                catch (IOException | ParseException e) { System.err.println(Arrays.toString(e.getStackTrace())); }
            });

            AtomicInteger count = new AtomicInteger(0);
            socketPlayerMap.forEach((clientSocket, player) -> {
                socketPlayerMap.get(clientSocket).setPosition(200 + count.incrementAndGet() * 20, 0);
            }); count.set(0);
        }

        if (GameStateManager.IM_CONNECTED && !GameStateManager.SERVER_IS_ONLINE) {
            File[] files = new File("players_income/").listFiles();

            for (int i = 0; i < files.length; i++) {
                try {
                    Player player = new Player(tileMap, "man", files[i]);
                    player.setPosition(200 + i * 20, 0);
                    players.add(player);
                } catch (IOException | ParseException e) { logger.info("sas" + e.getMessage()); }
            }
        }

        isAvailable = true;
    }

    @Override
    public void update() {
        if (GameStateManager.SERVER_IS_ONLINE) {
            AtomicInteger count = new AtomicInteger(0);
            socketPlayerMap.forEach((socket, player) -> {
                player.update();
                player.getPlayerUtilities().savePlayer(count.incrementAndGet());
            });
            count.set(0);
            gsm.getServer().sendUpdatedPlayers();
        }
        if (GameStateManager.IM_CONNECTED && !GameStateManager.SERVER_IS_ONLINE) {
            File[] files = new File("players_income/").listFiles();
            for (int i = 0; i < players.size(); i++) {
                try {
                    players.get(i).getPlayerUtilities().loadPlayer(files[i]);
                } catch (IOException | ParseException e) {
                    e.printStackTrace();
                }
            }
        }
        bg.setPosition(tileMap.getx(), tileMap.gety());
    }

    @Override
    public void draw(Graphics2D g) {
        //draw state
        bg.draw(g);
        tileMap.draw(g);

        //draw players
        if (GameStateManager.SERVER_IS_ONLINE)
            socketPlayerMap.forEach((clientSocket, player) -> player.draw(g));
        else
            players.forEach(player -> player.draw(g));
    }

    @Override
    public void keyPressed_(Socket client, int keyCode) {
        if (keyCode == KeyEvent.VK_A) socketPlayerMap.get(client).setLeft(true);
        if (keyCode == KeyEvent.VK_D) socketPlayerMap.get(client).setRight(true);
        if (keyCode == KeyEvent.VK_SPACE) socketPlayerMap.get(client).setJumping(true);
        if (keyCode == KeyEvent.VK_V) socketPlayerMap.get(client).setKnifeHit(true);
    }

    @Override
    public void keyReleased_(Socket client, int keyCode) {
        if (keyCode == KeyEvent.VK_A) socketPlayerMap.get(client).setLeft(false);
        if (keyCode == KeyEvent.VK_D) socketPlayerMap.get(client).setRight(false);
        if (keyCode == KeyEvent.VK_SPACE) socketPlayerMap.get(client).setJumping(false);
        if (keyCode == KeyEvent.VK_V) socketPlayerMap.get(client).setKnifeHit(false);
    }

    //doesn't use for coop
    @Override
    public void keyPressed_(int keyCode) {

    }

    @Override
    public void keyReleased_(int keyCode) {

    }

    @Override
    public void setPlayer(Player player) {

    }
}
