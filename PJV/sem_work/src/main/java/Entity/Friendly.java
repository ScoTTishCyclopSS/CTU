package Entity;

import TileMap.TileMap;

import java.awt.*;

/**
 * Represents the friendly.
 * @author shirover
 */
public class Friendly extends MapObject {

    /**
     * Creates the friendly object.
     * @param tm The TileMap object.
     */
    public Friendly(TileMap tm) {
        super(tm);
    }

    /**
     * Updates the NPC.
     */
    public void update() { }

    /**
     * Draws the NPC.
     * @param g The Graphics class, used to draw.
     */
    public void draw(Graphics2D g) {
        super.draw(g);
    }
}

