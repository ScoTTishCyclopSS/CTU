package Entity;

import java.awt.image.BufferedImage;

/**
 * Represents the animation process.
 * @author shirover
 * @author timusfed
 */
public class AnimationStuff {

    private BufferedImage[] frames;
    private int currentFrame;

    private long startTime;
    private long delay;

    private boolean playedOnce;

    /**
     * Creates the new animation object.
     * Sets the the 'played once' statement.
     */
    public AnimationStuff() {
        playedOnce = false;
    }

    /**
     * Sets the frames to animate.
     * @param frames The BufferedImage array, representing the frames to play.
     */
    public void setFrames(BufferedImage[] frames) {
        this.frames = frames;
        currentFrame = 0;
        startTime = System.nanoTime();
        playedOnce = false;
    }

    /**
     * Sets delay on an current animation.
     * @param delay The long, representing delay in ms.
     */
    public void setDelay(long delay) {
        this.delay = delay;
    }

    /**
     * Updates the animation loop.
     */
    public void update() {

        if (delay == -1) return;

        long elapsed = (System.nanoTime() - startTime) / 1000000;
        if (elapsed > delay) {
            currentFrame++;
            startTime = System.nanoTime();
        }
        if (currentFrame == frames.length) {
            currentFrame = 0;
            playedOnce = true;
        }
    }

    /**
     * Gets the specified image from the frames array.
     * @return frames[currentFrame] The BufferedImage, representing image.
     */
    public BufferedImage getImage() {
        return frames[currentFrame];
    }

    /**
     * Checks if animation was played once.
     * @return playedOnce The boolean, representing the fact of 'played once' statement.
     */
    public boolean hasPlayedOnce() { return playedOnce; }
}
