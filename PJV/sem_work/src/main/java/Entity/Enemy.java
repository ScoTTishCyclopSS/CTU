package Entity;

import Coop.Server;
import TileMap.TileMap;

import java.awt.*;
import java.util.logging.Logger;

/**
 * Represents the Enemy.
 * @author shirover
 */
public class Enemy extends MapObject {

    Logger logger = Logger.getLogger(Server.class.getName());

    protected int health;
    protected int maxHealth;
    protected boolean dead;
    protected int damage;
    protected boolean flinch;
    protected long flinchTimer;

    /**
     * Creates the Enemy object
     * @param tm Represents the TileMap
     */
    public Enemy(TileMap tm) {
        super(tm);
    }

    /**
     * Gets the enemy status.
     * @return dead The boolean, representing the 'is dead' statement.
     */
    public boolean isDead() {
        return dead;
    }

    /**
     * Gets the damage, given from the enemy to the player.
     * @return damage The int, representing the damage.
     */
    public int getDamage() {
        return damage;
    }

    /**
     * Dealing damage to an enemy.
     * @param damage The int, representing the damage given to the enemy.
     */
    public void hit(int damage) {

        if (dead || flinch) {
            return;
        }

        health -= damage;
        logger.info("Enemy get damage!");

        if (health < 0) {
            health = 0;
        }

        if (health == 0) {
            dead = true;
        }

        flinch = true;
        flinchTimer = System.nanoTime();
    }

    /**
     * Updates the enemy.
     */
    public void update () {

    }

    /**
     * Gets the direction of the enemy.
     * @return facingRight The boolean, representing the 'facing right' statement.
     */
    public boolean getDirection() {return facingRight;}

    /**
     * Draws the enemy.
     * @param g The Graphics class, used to draw.
     */
    public void draw(Graphics2D g) {
        super.draw(g);
    }

    /**
     * Gets the health of the enemy.
     * @return health The int, representing the health.
     */
    public int getHealth() {
        return health;
    }
}

