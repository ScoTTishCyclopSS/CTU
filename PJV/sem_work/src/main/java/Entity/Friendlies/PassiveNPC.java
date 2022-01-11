package Entity.Friendlies;

import Entity.AnimationStuff;
import Entity.Bullet;
import Entity.Enemy;
import Entity.Friendly;
import TileMap.TileMap;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Objects;

/**
 * Represents NPC in game.
 * @author shirover
 */
public class PassiveNPC extends Friendly {

    private static boolean speak;
    private String name;
    private BufferedImage[] dlg;

    /**
     * Creates new passive NPC.
     * @param tm TileMap object
     * @param name String, representing NPC's name
     */
    public PassiveNPC(TileMap tm, String name) {
        super(tm);
        moveSpeed = 1.3;
        maxSpeed = 1.3;
        fallSpeed = 0.2;
        maxFallSpeed = 10.0;

        width = 120;
        height = 120;
        cwidth = 120;
        cheight = 90;

        speak = false;

        this.name = name;
        try {
            loadSprites();
        } catch (IOException e) {
            System.err.println(Arrays.toString(e.getStackTrace()));
        }
        facingRight = false;

    }

    private void loadSprites() throws IOException {

        //load sprites of enemy
        BufferedImage NPCSheet = ImageIO.read(
                Objects.requireNonNull(
                        getClass().getResourceAsStream("/Friendlies/grandpa.gif")));
        BufferedImage[] sprites = new BufferedImage[NPCSheet.getWidth() / width];
        for (int i = 0; i < sprites.length; i++) {
            sprites[i] = NPCSheet.getSubimage(i * width, 0, width, height);
        }


        BufferedImage menuBeans = ImageIO.read(
                Objects.requireNonNull(
                        getClass().getResourceAsStream("/Dialogs/speak.gif")));
        dlg = new BufferedImage[1];
        dlg[0] = menuBeans.getSubimage(0, 0, 1200, 720);

        animation = new AnimationStuff();
        animation.setFrames(sprites);
        animation.setDelay(200);
    }


    private void getNextPos() {
        if (falling) {
            dy += fallSpeed;
        }
    }

    /**
     * Updates NPC via parameters such as position, collision, animation.
     */
    public void update() {
        getNextPos();
        checkCollision();
        setPosition(xtemp, ytemp);

        // update animation
        animation.update();
    }

    /**
     * Draws the NPC.
     * @param g The Graphics class, used to draw.
     */
    public void draw(Graphics2D g) {
        setMapPosition();
        super.draw(g);
        if (speak) {
            g.drawImage(dlg[0], 0, -25, null);
            Font font = new Font("Arial", Font.PLAIN, 19);
            g.setFont(font);
            g.setColor(Color.BLACK);

            g.drawString("- How do you call the big bunch of cats?", 495, 570);
            g.drawString("- How?", 495, 590);
            g.drawString("- The meowntain!", 495, 610);
            g.drawString("- Purr-fect.", 495, 630);
        }
    }

    /**
     * Sets 'speak' statement.
     * @param speak The boolean, representing 'speaking' action.
     */
    public static void setSpeak(boolean speak) {
        PassiveNPC.speak = speak;
    }
}