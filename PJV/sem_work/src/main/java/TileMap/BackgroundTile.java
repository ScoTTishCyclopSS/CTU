package TileMap;

import main.GamePanel;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.util.Arrays;

/**
 * Represents the Background tool.
 * @author shirover
 */
public class BackgroundTile {

    private BufferedImage img;
    private double x, y, dx, dy, moveScale;

    /**
     * Creates the new BackgroundTile.
     * Sets the image by income path,
     * @param path The String, representing the path to the image background file.
     * @param scale The int, representing the move scale (for camera movement effect)
     */
    public BackgroundTile(String path, double scale) {
        try {
            img = ImageIO.read(getClass().getResourceAsStream(path));
            moveScale = scale;
        } catch (IOException e) {
            System.err.println(Arrays.toString(e.getStackTrace()));
        }
    }

    /**
     * Updates the position of the backgroud scale.
     * @param path The String, representing the path to the image background file.
     */
    public void update(String path) {
        try {
            img = ImageIO.read(getClass().getResourceAsStream(path));
        } catch (IOException e) { System.err.println(Arrays.toString(e.getStackTrace()));}
        x += dx;
        y += dy;
    }

    /**
     * Draws the background, depends on his scale.
     * @param g The Graphics class, used to draw.
     */
    public void draw(Graphics2D g) {
        g.drawImage(img, (int)x, (int)y, null);
        if (x < 0)
            g.drawImage(img, (int)x + GamePanel.WIDTH, (int)y, null);
        if (x > 0)
            g.drawImage(img, (int)x - GamePanel.WIDTH, (int)y, null);
    }

    /**
     * Sets the background position on a screen.
     * @param x The int, representing the X coordinate.
     * @param y The int, representing the Y coordinate.
     */
    public void setPosition(double x, double y) {
        this.x = (x * moveScale) % GamePanel.WIDTH;
        this.y = (y * moveScale) % GamePanel.HEIGHT;
    }
}
