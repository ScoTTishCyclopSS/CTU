package Entity.Loot;

import Entity.MapObject;
import TileMap.TileMap;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Objects;

import static GameState.GameState.TILE_SIZE;

/**
 * Represents the container with loot in game.
 * @author timusfed
 */
public class LootContainer extends MapObject {

    /**
     * Represents the type of the container as box.
     * Could be more of this.
     */
    public static String BOX_CONTAINER = "box";

    private final int slots;
    private final ArrayList<Loot> loot;
    private BufferedImage texture;

    private int strength;
    private boolean isDestroyed;

    private boolean flinch;
    private long flinchTimer;

    /**
     * Creates new container.Sets it's parameters.
     * Sets container as empty.
     * @param tm TileMap object.
     * @param slotsCount Represents the capacity of the container.
     * @param type String, representing the type of the container.
     */
    public LootContainer(TileMap tm, int slotsCount, String type) {
        super(tm);
        slots = slotsCount;
        loot = new ArrayList<>();
        facingRight = true;
        fallSpeed = 0.2;
        maxFallSpeed = 10.0;
        width = height = TILE_SIZE;
        cwidth = cheight = TILE_SIZE - 10;
        strength = 16; // 2 knife hits
        isDestroyed = false;

        try {
            texture = ImageIO.read(
                    Objects.requireNonNull(
                            getClass().getResourceAsStream(
                                    "/Loot/containers/" + type + ".gif")));
        } catch (IOException e) {
            System.err.println(Arrays.toString(e.getStackTrace()));
        }
    }

    /**
     * Updates the physics and statement 'is destroyed' of the container
     */
    public void update() {
        getNextPos();
        checkCollision();
        setPosition(xtemp, ytemp);

        // check flinch
        if (flinch) {
            long elepsed = (System.nanoTime() - flinchTimer) / 1000000;
            if (elepsed > 400) {
                flinch = false;
            }
        }
    }

    /**
     * Represents the fact of hitting the container.
     * Is immune to all types of attacks, except for melee attacks
     * @param damage The int, representing the damage, dealing to the container.
     */
    public void hit(int damage) {

        if (isDestroyed || flinch)
            return;

        strength -= damage;

        if (strength < 0)
            strength = 0;

        if (strength == 0) {
            isDestroyed = true;
        }

        flinch = true;
        flinchTimer = System.nanoTime();
    }

    private void getNextPos() {
        if (falling) {
            dy += fallSpeed;
        }
    }

    /**
     * Draws the container.
     * @param g The Graphics class, used to draw.
     */
    public void draw(Graphics2D g) {
        setMapPosition();
        g.drawImage(texture, (int) (x + xmap - width / 2),
                (int) (y + ymap - height / 2), width, height, null);
    }

    /**
     * Puts the oot unit inside the container, unless it doesn't have space left for it.
     * @param unit Represents the one loot unit.
     */
    public void putLoot(Loot unit) {
        if (loot.size() < slots)
            loot.add(unit);
    }

    /**
     * Gets the 'is destroyed' statement of the container.
     * @return isDestroyed The boolean, representing the 'is destroyed' statement.
     */
    public boolean isDestroyed() {
        return isDestroyed;
    }

    /**
     * Gets the loot inside the container.
     * @return loot An array, representing the loot.
     */
    public ArrayList<Loot> getLoot() {
        return loot;
    }
}
