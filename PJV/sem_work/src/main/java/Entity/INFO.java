package Entity;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Objects;

/**
 * Represents the information bar of the player.
 * @author shirover
 * @author timusfed
 */
public class INFO {

    private final Player player;
    private BufferedImage healthSheet;
    private BufferedImage shieldSheet;
    private final int width;

    /**
     * Creates the new INFO object.
     * Sets the player, width.
     * Loads textures for health and shields.
     * @param p Player object
     */
    public INFO(Player p) {
        player = p;
        width = 24;
        try {
            healthSheet = ImageIO.read(Objects.requireNonNull(
                    getClass().getResourceAsStream("/INFO/health.gif")));
            shieldSheet = ImageIO.read(Objects.requireNonNull(
                    getClass().getResourceAsStream("/INFO/shield.gif")));
        } catch (IOException e) {
            System.err.println(Arrays.toString(e.getStackTrace()));
        }
    }

    /**
     * Draws the info to the screen.
     * @param g The Graphics class, used to draw.
     */
    public  void draw (Graphics2D g) {
        for (int i = 0; i < player.getHealth(); i++)
            g.drawImage(healthSheet, i * width, 20, null);
        for (int j = 0; j < player.getShield(); j++)
            g.drawImage(shieldSheet, j * width, 50, null);

        g.setColor(Color.WHITE);
        g.drawString(player.getFire()/100  + "/" + player.getMaxFire()/100 , 50, 122);
    }
}

