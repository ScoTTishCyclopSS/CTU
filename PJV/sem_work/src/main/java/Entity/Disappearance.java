package Entity;


import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.util.Arrays;
import java.util.Objects;

/**
 * Represents the disappearance of the MapObject.
 * @author shirover
 */
public class Disappearance {

    private final int x;
    private final int y;
    private int xmap;
    private int ymap;

    private final int width;
    private final int height;

    private AnimationStuff animation;

    private boolean remove;
    private final boolean direction;

    /**
     * Creates the new Disappearance object.
     * @param dirct Represents the direction of the object.
     * @param x Represents the X coordinate of the object.
     * @param y Represents the Y coordinate of the object.
     */
    public Disappearance(boolean dirct, int x, int y) {
        this.x = x;
        this.y = y;

        width = 120;
        height = 120;
        direction = dirct;

        try {
            loadSprites();
        } catch (IOException e) {
            System.err.println(Arrays.toString(e.getStackTrace()));
        }

    }

    /**
     * Loads the sprites of the disappearance frames.
     * Sets the to the array and delay.
     * @throws IOException Exception on non-existing files.
     */
    public void loadSprites() throws IOException {
        BufferedImage DisappearanceSheet = ImageIO.read(
                Objects.requireNonNull(
                        getClass().getResourceAsStream("/Enemies/enemy_disap.gif")));
        BufferedImage[] sprites = new BufferedImage[DisappearanceSheet.getWidth() / width];
        for (int i = 0; i < sprites.length; i++) {
            sprites[i] = DisappearanceSheet.getSubimage(i * width, 0, width, height);
        }

        animation = new AnimationStuff();
        animation.setFrames(sprites);
        animation.setDelay(70);
    }

    /**
     * Updates the animation loop.
     */
    public void update() {
        animation.update();
        if(animation.hasPlayedOnce()) {
            remove = true;
        }
    }

    /**
     * Remove the End the disappearance animation, if flag to remove is set.
     * @return remove The boolean, representing the 'to remove' statement.
     */
    public boolean removing() {
        return remove;
    }

    /**
     * Sets the position of the disappearance animation.
     * Often the same as object position.
     * @param x The int, representing the X coordinate.
     * @param y The int, representing the Y coordinate.
     */
    public void setMapPosition(int x, int y) {
        xmap = x;
        ymap = y;
    }

    /**
     * Draws the disappearance animation.
     * @param g The Graphics class, used to draw.
     */
    public void draw(Graphics2D g) {
        if (direction) {
            g.drawImage(
                    animation.getImage(),
                    (int) (x + xmap - width / 2),
                    (int) (y + ymap - height / 2),
                    width, height, null
            );
        } else {
            g.drawImage(
                    animation.getImage(),
                    (int) (x + xmap + width / 2),
                    (int) (y + ymap - height / 2),
                    -width, height, null
            );
        }
    }
}