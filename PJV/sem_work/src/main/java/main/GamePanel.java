package main;

import Coop.Server;
import GameState.GameStateManager;

import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.image.BufferedImage;
import java.util.Arrays;

/** Represents a Game itself.
 * @author timusfed
 * @author shirover
 */
public class GamePanel extends JPanel implements Runnable, KeyListener {

    /**
     * Represents the preferred width of the game's window.
     */
    public static final int WIDTH = 1200;
    /**
     * Represents the preferred height of the game's window.
     */
    public static final int HEIGHT = 720;

    /**
     * Represents the thread to run the game.
     */
    private Thread thread;
    /**
     * Represents the status of the game thread.
     */
    private boolean isRunning;

    /**
     * Represents the 'frame', which will be drawn.
     */
    private BufferedImage img;
    /**
     * The Graphics class is the abstract base class for all graphics contexts
     * that allow an application to draw onto components that are realized on various devices,
     * as well as onto off-screen images.
     * @see <a href="https://docs.oracle.com/javase/7/docs/api/java/awt/Graphics2D.html">Graphics2D</a>
     */
    private Graphics2D g;

    /**
     * Represents the GameStateManager.
     */
    private GameStateManager gsm;

    /**
     * Creates the GamePanel with preferred size of the panel.
     */
    public GamePanel() {
        super();
        setPreferredSize(new Dimension(WIDTH, HEIGHT));
        setFocusable(true); // focus on frame
    }

    /**
     * Filling the panel up and draws it.
     * Starting the thread up, also creates the new GameStateManager.
     */
    public void init() {
        img = new BufferedImage(WIDTH, HEIGHT, BufferedImage.TYPE_INT_RGB);
        g = (Graphics2D) img.getGraphics();
        isRunning = true;
        gsm = new GameStateManager();
    }

    /**
     * JComponent's .addNotify() method.
     * @see <a href="https://docs.oracle.com/javase/6/docs/api/java/awt/Component.html#addNotify%28%29">Component</a>
     */
    public void addNotify() {
        super.addNotify();
        if (thread == null) {
            thread = new Thread(this);
            addKeyListener(this);
            thread.start();
            isRunning = true;
        }
    }

    /**
     * Run the cycle of the game.
     * Represents the continuous update of every frame in game.
     * Also dynamically sets FPS based on a 'time', which is need to draw everything.
     * FPS represents in Thread.sleep('time') method.
     */
    @Override
    public void run() {
        init();
        long start;
        long elapsed;
        long wait;

        while (isRunning) {
            start = System.nanoTime();
            update();
            draw();
            drawToScreen(); // game's panel graph obj

            elapsed = System.nanoTime() - start; // how much time did get update + draw + draw to screen
            //
            long targetTime = 1000 / 60;
            wait = targetTime - elapsed / 1000000; //??

            if (wait < 0) {
                wait = 5;
            }
            try {
                Thread.sleep(wait);
            } catch (InterruptedException e) {
                System.err.println(Arrays.toString(e.getStackTrace())); // if thread.sleep run out of time => exception
            }

            Graphics g2 = getGraphics();
            g2.drawImage(img, 0, 0, null);
            g2.dispose(); //close graphics
        }
    }

    /**
     * The part of the run() cycle.
     * Draw picture (frame) directly to the screen.
     */
    private void drawToScreen() {
        Graphics g2 = getGraphics();
        g2.drawImage(img, 0, 0, null);
        g2.dispose();
    }

    /**
     * Updating the GameStateManager.
     */
    private void update() {
        gsm.update();
    }

    /**
     * Calls the GameStateManager's draw() method.
     */
    private void draw() {
        gsm.draw(g);
    }

    @Override
    public void keyTyped(KeyEvent keyEvent) {

    }

    @Override
    public void keyPressed(KeyEvent keyEvent) {
        gsm.keyPressed(keyEvent);
    }

    @Override
    public void keyReleased(KeyEvent keyEvent) {
        gsm.keyReleased(keyEvent);
    }
}
