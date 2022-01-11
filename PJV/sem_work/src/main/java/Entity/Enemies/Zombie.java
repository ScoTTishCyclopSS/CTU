package Entity.Enemies;

import Entity.AnimationStuff;
import Entity.Enemy;
import TileMap.TileMap;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.util.Arrays;

/**
 * Represents the type of enemy - zombie.
 * Zombie is an enemy with close attack.
 * Deals damage nly when player is nearby, about 1 tile or less.
 * @author shirover
 */
public class Zombie extends Enemy {

    private BufferedImage[] sprites;

    /**
     * Creates new Zombie.
     * @param tm TileMpa object.
     */
    public Zombie(TileMap tm) {
        super(tm);

        moveSpeed = 1.3;
        maxSpeed = 1.3;
        fallSpeed = 0.2;
        maxFallSpeed = 10.0;

        width = 120;
        height = 120;
        cwidth = 40;
        cheight = 100;

        health = maxHealth = 15;
        damage = 1;

        try {
            loadEnemy();
        } catch (IOException e) {
            System.err.println(Arrays.toString(e.getStackTrace()));
        }

    }

    private void loadEnemy() throws IOException {

        //load sprites of enemy
        BufferedImage EnemySheet = ImageIO.read(getClass().getResourceAsStream("/Enemies/enemy1.gif"));
        sprites = new BufferedImage[EnemySheet.getWidth() / width];
        for (int i = 0; i < sprites.length; i++) {
            sprites[i] = EnemySheet.getSubimage(i * width, 0, width, height);
        }

        animation = new AnimationStuff();
        animation.setFrames(sprites);
        animation.setDelay(90);

        right = true;

    }

    private void getNextPosition() {

        if (left) {
            dx -= moveSpeed;
            facingRight = false;
            if (dx < -maxSpeed)
                dx = -maxSpeed;
        } else if (right) {
            dx += moveSpeed;
            facingRight = true;
            if (dx > maxSpeed)
                dx = maxSpeed;
        }

        if (falling) {
            dy += fallSpeed;
        }

    }

    /**
     * Updates zombie.
     * His characteristics such as health or flinching statement.
     * And physics such as position, collision.
     * Also movement and animation.
     */
    public void update() {

        // update enemy position
        getNextPosition();
        checkCollision();
        setPosition(xtemp, ytemp);

        // check flinch
        if (flinch) {
            long elepsed = (System.nanoTime() - flinchTimer) / 1000000;
            if (elepsed > 400) {
                flinch = false;
            }
        }

        // if enemy hits a wall, go other direction
        if (right && dx == 0 ) {
            right = false;
            left = true;
        }
        else if ( left && dx == 0) {
            left = false;
            right = true;
        }

        // update animation
        animation.update();
    }

    /**
     * Draws zombie.
     * @param g The Graphics class, used to draw.
     */
    public void draw(Graphics2D g) {

        //if (!isOnScreen()) return;
        setMapPosition();
        super.draw(g);

    }


}
