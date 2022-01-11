package Entity;

import TileMap.TileMap;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.util.Arrays;

/**
 * Represents the bullet.
 * @author shirover
 */
public class Bullet extends MapObject {

    private boolean hit;
    private boolean remove;
    private BufferedImage[] hitSprites;


    // the direction of the shot is taken from the player

    /**
     * Creates the bullet object.
     * @param tm The tile map object.
     * @param right The boolean, representing the direction of the shot is taken from the player.
     */
    public Bullet(TileMap tm, boolean right) {
        super(tm);

        facingRight = right;

        moveSpeed = 25.8;
        if (right) dx = moveSpeed;
        else dx = -moveSpeed;

        width = 12;
        height = 12;
        cwidth = 12;
        cheight = 12;

        try {
            loadBullet();
        } catch (IOException e) {
            System.err.println(Arrays.toString(e.getStackTrace()));
        }
    }

    /**
     * Loads the bullet textures.
     * @throws IOException Exception on non-existing files.
     */
    public void loadBullet() throws IOException {

        // load bullet flying sprite
        BufferedImage bulletSheet = ImageIO.read(getClass().getResourceAsStream("/weapons/bullet.gif"));
        BufferedImage[] sprites = new BufferedImage[bulletSheet.getWidth() / width];
        for (int i = 0; i < sprites.length; i++) {
            sprites[i] = bulletSheet.getSubimage(i * width, 0, width, height);
        }
        // load bullet hitting sprite
        BufferedImage hitSheet = ImageIO.read(getClass().getResourceAsStream("/weapons/bullet_Sheet.gif"));
        hitSprites = new BufferedImage[hitSheet.getWidth() / width];
        for (int i = 0; i < hitSprites.length; i++) {
            hitSprites[i] = hitSheet.getSubimage(i * width, 0, width, height);
        }

        animation = new AnimationStuff();
        animation.setFrames(sprites);
        animation.setDelay(10);
    }

    /**
     * If bullet hits something, set hit and start her disappearance.
     */
    public void setHit() {
        if (hit) return;
        hit = true;
        animation.setFrames(hitSprites);
        animation.setDelay(30);
        dx = 0;
    }

    /**
     * Gets remove state of the bullet.
     * @return remove The boolean, representing the state of removing the bullet.
     */
    public boolean removing() {
        return remove;
    }

    /**
     * Updates the bullet statement. Such as position, animation etc.
     */
    public void update() {

        checkCollision();
        setPosition(xtemp, ytemp);

        if (dx == 0 && !hit) setHit();

        animation.update();

        if (hit && animation.hasPlayedOnce()) remove = true;

    }

    /**
     * Draws the bullet on a screen.
     * @param g The Graphics class, used to draw.
     */
    public void draw(Graphics2D g) {
        setMapPosition();
        super.draw(g);
    }
}
