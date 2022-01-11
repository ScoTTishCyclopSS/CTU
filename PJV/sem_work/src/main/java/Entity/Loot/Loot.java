package Entity.Loot;

import Coop.Server;
import Entity.MapObject;
import Entity.Player;
import TileMap.TileMap;
import sun.rmi.runtime.Log;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.util.Arrays;
import java.util.Objects;
import java.util.logging.Logger;

import static GameState.GameState.TILE_SIZE;

/**
 * Represents the loot unit in game.
 * @author timusfed
 */
public class Loot extends MapObject {

    /**
     * Represents the type of loot as 'armor'.
     */
    public final static String ARMOR_LOOT = "armor";
    /**
     * Represents the type of loot as 'supply'.
     */
    public final static String SUPPLY_LOOT = "supply";
    /**
     * Represents the type of loot as 'guns'.
     */
    public final static String GUNS_LOOT = "guns";

    private BufferedImage texture;
    private final String type;

    /**
     * Represents the name of unit.
     */
    public final String unit;
    /**
     * Represents the texture of unit, used in inventory.
     */
    public BufferedImage invTexture;
    /**
     * Represents the slot in hotbar of player, where can be placed the unit.
     */
    public int slot;

    private final Logger logger = Logger.getLogger(Loot.class.getName());

    /**
     * Creates the new loot unit.
     * Sets the basic physics for it.
     * @param tm TileMap object
     * @param type String, representing type of loot
     * @param unit String, representing 'name' of the unit. For example: "beans", "m4". Used for searching textures in files.
     */
    public Loot(TileMap tm, String type, String unit) {
        super(tm);
        facingRight = true;
        this.unit = unit;
        if (type.equals(SUPPLY_LOOT))
            width = height = cwidth = cheight = 19;

        if (type.equals(ARMOR_LOOT) || type.equals(GUNS_LOOT)) {
            width = height = TILE_SIZE;
            cwidth = cheight = 15;
        }

        fallSpeed = 0.2;
        maxFallSpeed = 10.0;
        this.type = type;
        slot = -1;

        try {
            texture = ImageIO.read(
                    Objects.requireNonNull(
                            getClass().getResourceAsStream(
                                    "/Loot/" + type + "/" + unit + ".gif")));
            invTexture = ImageIO.read(
                    Objects.requireNonNull(
                            getClass().getResourceAsStream(
                                    "/Loot/" + type + "/for_menu/" + unit + "_menu.gif")));
        } catch (IOException e) {
            System.err.println(Arrays.toString(e.getStackTrace()));
        }
    }

    /**
     * Updates the loot unit.
     * Basically physics
     */
    public void update() {
        getNextPos();
        checkCollision();
        setPosition(xtemp, ytemp);
    }

    private void getNextPos() {
        if (falling) {
            dy += fallSpeed;
        }
    }

    /**
     * Draws the unit.
     * @param g The Graphics class, used to draw from.
     */
    public void draw(Graphics2D g) {
        setMapPosition();
        g.drawImage(texture, (int) (x + xmap - width / 2),
                (int) (y + ymap - height / 2), width, height, null);
    }

    /**
     * Each type of loot has its own action on player, who cares the unit.
     * Method performs an action on player directly.
     * For example: food gives health, armor gives shields.
     * @param player Represents the player
     */
    public void action(Player player) {
        if (type.equals(Loot.SUPPLY_LOOT) && player.getHealth() != player.getMaxHealth()) {
            player.setHealth(player.getHealth() + 1);
            logger.info(player + "health + 1: " + player.getHealth());
        } else if (type.equals(Loot.ARMOR_LOOT) && unit.equals("helmet")) {
            player.setShield(player.getShield() + 2);
            logger.info(player + " now my head is bulletproof?");
            player.getPlayerHotBar().set(Player.HELMET_SLOT, this);
            slot = Player.HELMET_SLOT;
        } else if (type.equals(Loot.ARMOR_LOOT) && unit.equals("vest")) {
            player.setShield(player.getShield() + 3);
            logger.info(player + " oh, I'm looking fat now...");
            player.getPlayerHotBar().set(Player.VEST_SLOT, this);
            slot = Player.VEST_SLOT;
        }else if (type.equals(Loot.GUNS_LOOT)) {
            logger.info(player + " what a shiny m4, babe!");
            player.getPlayerHotBar().set(Player.GUN_SLOT, this);
            slot = Player.GUN_SLOT;
        }
    }

    /**
     * Gets the type of the unit.
     * @return type String, representing,the type of the loot unit.
     */
    public String getType() {
        return type;
    }
}
