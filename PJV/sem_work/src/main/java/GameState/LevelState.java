package GameState;


import Entity.*;
import Entity.Enemies.Zombie;
import Entity.Friendlies.PassiveNPC;
import Entity.Loot.Loot;
import Entity.Loot.LootContainer;
import Sound.SoundBoard;
import TileMap.BackgroundTile;
import TileMap.TileMap;
import main.GamePanel;
import org.json.simple.parser.ParseException;

import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyEvent;
import java.io.IOException;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.logging.Logger;

import static Entity.Loot.LootContainer.BOX_CONTAINER;

/**
 * Represents the playable state of the game.
 * @author timusfed
 * @author shirover
 */
public class LevelState extends GameState {

    private Player player;
    private SoundBoard soundBoard;
    private final JFrame frameOnDeath;
    private JPanel panel;
    private final ArrayList<Enemy> enemies;
    private ArrayList<Disappearance> disappearances;
    private final ArrayList<PassiveNPC> npc;
    private INFO info;
    private final ArrayList<Loot> loot;
    private final ArrayList<LootContainer> containers;
    private final String name;
    private Teleport teleport;

    /**
     * Creates the new level object.
     * Initialise the Loot, Containers, Death Frames,
     * Local panel (death dialog), Teleport, Enemies array, Friendlies array.
     * Sets the GameStateManager, Name, Availability.
     * @param gsm Represents the GameStateManager.
     * @param name String, representing the name of the state. Like "basement","castle" etc.
     */
    public LevelState(GameStateManager gsm, String name) {
        this.gsm = gsm;
        this.name = name;

        loot = new ArrayList<>();
        containers = new ArrayList<>();
        enemies = new ArrayList<>();
        npc = new ArrayList<>();
        frameOnDeath = new JFrame();
        panel = null;
        teleport = null;

        isAvailable = false;
    }

    @Override
    public void init() {
        tileMap = new TileMap(TILE_SIZE); // the one tile size
        tileMap.loadTiles("/testmap/" + name + "/tileset.gif"); // load the tiles from the file
        tileMap.loadMap("/testmap/" + name + "/map.map"); // load the map for the current state
        tileMap.setTween(0.07);
        bg = new BackgroundTile("/Backgrounds/" + name + "/test.png", 0.1); // just the background

        try {
            player = new Player(tileMap, gsm.getPlayerSex(), null);

            loot.add(new Loot(tileMap, Loot.SUPPLY_LOOT, "beans"));
            loot.add(new Loot(tileMap, Loot.ARMOR_LOOT, "helmet"));
            loot.add(new Loot(tileMap, Loot.ARMOR_LOOT, "vest"));
            loot.add(new Loot(tileMap, Loot.GUNS_LOOT, "m4"));

            containers.add(new LootContainer(tileMap, 10, BOX_CONTAINER));
            containers.get(0).putLoot(new Loot(tileMap, Loot.SUPPLY_LOOT, "beans"));
            containers.get(0).putLoot(new Loot(tileMap, Loot.ARMOR_LOOT, "helmet"));

        } catch (IOException | ParseException e) {
            System.err.println(Arrays.toString(e.getStackTrace()));
        }

        tileMap.setPosition(0, 0); // where to draw tiles
        player.setPosition(120, 400); // where to draw player
        containers.forEach(container -> container.setPosition(470, TILE_SIZE * 4 - container.getCheight()));

        AtomicInteger count = new AtomicInteger(0);
        loot.forEach(unit -> unit.setPosition(150 + count.incrementAndGet() * 30, 450));
        count.set(0);

        createEnemies();
        createNPC();

        disappearances = new ArrayList<>();

        info = new INFO(player);

        teleport = new Teleport(49);

        isAvailable = true;
    }

    /**
     * Creates enemies on current positions.
     * Position set by user with the Pint class (recommended).
     * Spawning with iterating over Point array.
     * @see <a href="https://docs.oracle.com/javase/7/docs/api/java/awt/Point.html"> Point </a>
     */
    public void createEnemies() {
        Zombie z;
        Point[] points = new Point[]{
                new Point(11 * 120, 200), new Point(17 * 120, 200), new Point(19 * 120, 200),
                new Point(29 * 120, 200), new Point(41 * 120, 200), new Point(42 * 120, 200)
        };
        for (Point point : points) {
            z = new Zombie(tileMap);
            z.setPosition(point.x, point.y);
            enemies.add(z);
        }
    }

    /**
     * Creates NPCs on current positions.
     * Position set by user with the Pint class (recommended).
     * Spawning with iterating over Point array.
     * @see <a href="https://docs.oracle.com/javase/7/docs/api/java/awt/Point.html"> Point </a>
     */
    public void createNPC(){
        PassiveNPC g;
        Point[] points = new Point[]{
                new Point(5 * 120, 200)
        };
        for (Point point : points) {
            g = new PassiveNPC(tileMap, "dedecek");
            g.setPosition(point.x, point.y);
            npc.add(g);
        }
    }

    @Override
    public void update() {
        //save player
        player.getPlayerUtilities().savePlayer(0);

        //update player
        player.update();

        //open the death screen on conditions
        if (player.isDead() && !frameOnDeath.isVisible()) {
            panel = player.getPlayerUtilities().deathPanel(frameOnDeath, gsm);
            frameOnDeath.setSize(new Dimension(150, 150));
            frameOnDeath.add(panel);
            frameOnDeath.setVisible(true);
        }

        //update tilemap
        tileMap.setPosition(GamePanel.WIDTH / 2 - player.getx(), GamePanel.HEIGHT / 2 - player.gety());

        //update loot
        loot.forEach(Loot::update);

        // set background, camera movement
        bg.setPosition(tileMap.getx(), tileMap.gety());

        // attack enemies
        player.checkAttack(enemies);

        // check dialogs with npc
        player.checkDialogs(npc);

        //check containers
        player.checkDestroy(containers);

        // update all enemies
        for (int i = 0; i < enemies.size(); i++) {
            Enemy e = enemies.get(i);
            e.update();
            if (e.isDead()) {
                boolean dirct = e.getDirection();
                enemies.remove(i);
                i--;
                disappearances.add(new Disappearance(dirct, e.getx(), e.gety()));
            }
        }

        //update all npc
        for (Friendly f : npc) {
            f.update();
        }

        // update disappearance of enemies
        for (int i = 0; i < disappearances.size(); i++) {
            disappearances.get(i).update();
            if (disappearances.get(i).removing()) {
                disappearances.remove(i);
            }
        }

        //update containers
        for (int i = 0; i < containers.size(); i++) {
            containers.get(i).update();
            if (containers.get(i).isDestroyed()) {
                ArrayList<Loot> insideLoot = containers.get(i).getLoot();
                for (int j = 0; j < insideLoot.size(); j++) {
                    insideLoot.get(j).setPosition(containers.get(i).getx() + j * 10, containers.get(i).gety());
                    loot.add(insideLoot.get(j));
                }
                containers.remove(i);
            }
        }

        //update teleporter
        if (teleport.getX() - TILE_SIZE / 2 < player.getx()) {
            teleport.initTeleportation(player, GameStateManager.BASEMENT_STATE, gsm);
            isAvailable = false;
        }
    }

    @Override
    public void draw(Graphics2D g) {
        // draw state
        bg.draw(g);
        tileMap.draw(g);
        containers.forEach(container -> container.draw(g));
        loot.forEach(supply -> supply.draw(g));

        //draw player
        player.draw(g);

        //draw enemies
        for (Enemy enemy : enemies)
            enemy.draw(g);

        //draw friendlies
        for(Friendly friendly : npc)
            friendly.draw(g);

        // draw disappearance
        for (Disappearance disappearance : disappearances) {
            disappearance.setMapPosition((int) tileMap.getx(), (int) tileMap.gety());
            disappearance.draw(g);
        }

        // draw info
        info.draw(g);
    }

    @Override
    public void keyPressed_(int k) {
        if (k == KeyEvent.VK_A) player.setLeft(true);
        if (k == KeyEvent.VK_D) player.setRight(true);
        if (k == KeyEvent.VK_SPACE) player.setJumping(true);
        if (k == KeyEvent.VK_ENTER) player.setShooting(true);
        if (k == KeyEvent.VK_V) player.setKnifeHit(true);
        if (k == KeyEvent.VK_R) player.setReload(true);

        if (k == KeyEvent.VK_ESCAPE) gsm.setState(GameStateManager.MENU_STATE); // escape menu alternative
        if (k == KeyEvent.VK_END) player.setDead(true); // for developers
        if (k == KeyEvent.VK_I) player.getInventory().init(loot);
        if (k == KeyEvent.VK_HOME) System.out.println(player.getInventory().getPlayerInventory()); // for developers
    }

    @Override
    public void keyReleased_(int k) {
        if (k == KeyEvent.VK_A) player.setLeft(false);
        if (k == KeyEvent.VK_D) player.setRight(false);
        if (k == KeyEvent.VK_SPACE) player.setJumping(false);
        if (k == KeyEvent.VK_ENTER) player.setShooting(false);
        if (k == KeyEvent.VK_V) player.setKnifeHit(false);
        if (k == KeyEvent.VK_R) player.setReload(false);
    }

    @Override
    public void setPlayer(Player player) {
        this.player = player;
    }

    //doesn't use for single player
    @Override
    public void keyPressed_(Socket socket, int keyCode) {

    }

    @Override
    public void keyReleased_(Socket socket, int keyCode) {
    }
}
