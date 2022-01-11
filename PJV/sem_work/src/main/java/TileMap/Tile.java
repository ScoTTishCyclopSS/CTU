package TileMap;

import java.awt.image.BufferedImage;

/**
 * Represents the one tile.
 * @author shirover
 */
public class Tile {

    private final BufferedImage image;
    private final int type; // to avoid confusion, not usable

    public static final int NORMAL = 0;
    /**
     * Represents the constant type of the tile as Blocked.
     * Possible to create as much as need. For example: "Normal", which is representing NON-Blocked type, or Air.
     */
    public static final int BLOCKED = 1;

    /**
     * Creates the new tile objects.
     * @param image The BufferedImage, representing tile's texture.
     * @param type The int, representing the type of the tile.
     */
    public Tile(BufferedImage image, int type) {
        this.image = image;
        this.type = type;
    }

    /**
     * gets the texture of the tile.
     * @return image The BufferedImage, representing tile's texture.
     */
    public BufferedImage getImage() {return image;}
}
