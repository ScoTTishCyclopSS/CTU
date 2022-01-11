package Entity;

import Coop.Server;
import Entity.Friendlies.PassiveNPC;
import Entity.Loot.Loot;
import Entity.Loot.LootContainer;
import Sound.SoundBoard;
import TileMap.TileMap;
import org.json.simple.parser.ParseException;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.logging.Logger;

/**
 * Represents Player.
 * @author timusfed
 * @author shirover
 */
public class Player extends MapObject {

    //logger
    Logger logger = Logger.getLogger(Server.class.getName());

    // player parameters
    private final PlayerUtilities playerUtilities;
    private int health;
    private int maxHealth;
    private int shield;
    private boolean dead;
    private String sex;

    // flinching
    private boolean flinch;
    private long flinchTimer;

    // shooting parameters
    private int fire;
    private int maxFire;
    private int fireCost;
    private int bulletDamage;
    private final ArrayList<Bullet> bullets;

    // knife parameters
    private int knifeDamage;
    private int knifeRange;

    //animation
    private final ArrayList<BufferedImage[]> frames;

    //inventory
    private final ArrayList<Loot> playerHotBar;
    private final Inventory inventory;

    //player states
    private static final int IDLE = 0;
    private static final int WALKING = 1;
    private static final int ATTACKING = 2;
    private static final int BULLET = 3;
    private static final int JUMPING = 4;

    /**
     * Represents helmet slot in player's hotbar.
     */
    public static final int HELMET_SLOT = 0;
    /**
     * Represents vest slot in player's hotbar.
     */
    public static final int VEST_SLOT = 1;
    /**
     * Represents gun slot in player's hotbar.
     */
    public static final int GUN_SLOT = 2;

    /**
     * Represents the set of clothes on player.
     */
    public static int LOOT_ON_ME = 1;

    private SoundBoard soundBoard;

    /**
     * Creates the new Player.
     * @param tm TileMap object.
     * @param sex Preferred sex.
     * @param param Param for the loadPlayer() method. "" if it's standard load of a new player.
     * @throws IOException Exception on reading or non-existing files.
     * @throws ParseException Parse file exception.
     */
    public Player(TileMap tm, String sex, File param) throws IOException, ParseException {
        super(tm);

        // loading player characteristics, then player
        playerUtilities = new PlayerUtilities(this);
        playerUtilities.loadPlayer(param);
        this.sex = sex;
        shooting = false;

        //create inventory
        inventory = new Inventory(this);
        playerHotBar = new ArrayList<>();
        for(int i = 0; i < 3; i++)
            playerHotBar.add(null);

        // create arrayLists for moving and bullet's sprites
        frames = new ArrayList<>();
        bullets = new ArrayList<>();

        // load sprites in arraylists
        try { playerUtilities.changeSprites(frames, 1, PlayerUtilities.LOAD); }
        catch (IOException e) { System.err.println(Arrays.toString(e.getStackTrace()));}

        // set start animation
        animation = new AnimationStuff();
        currentAction = IDLE;
        animation.setFrames(frames.get(IDLE));
        animation.setDelay(300);

        soundBoard = new SoundBoard("pistol");
    }

    /**
     * Checks if the dialog with an NPC is available.
     * @param npc An array of NPCs.
     */
    public void checkDialogs(ArrayList<PassiveNPC> npc) {
        for(Friendly friendly : npc)
            PassiveNPC.setSpeak(intersects(friendly));
    }

    /**
     * Checks if the container is broken.
     * @param containers An array of containers.
     */
    public void checkDestroy(ArrayList<LootContainer> containers) {
        // loop through containers
        for (LootContainer container : containers) {
            // check knife attack
            if (knifeHit) {
                if (facingRight) {
                    if (container.getx() > x &&
                            container.getx() < x + knifeRange &&
                            container.gety() > y - height / 2 &&
                            container.gety() < y + height / 2)
                    {
                        container.hit(knifeDamage);
                    }
                } else {
                    if (container.getx() < x &&
                            container.getx() > x - knifeRange &&
                            container.gety() > y - height / 2 &&
                            container.gety() < y + height / 2) {
                        container.hit(knifeDamage);
                    }
                }
            }
        }
    }

    /**
     * Compares the player's actions with attacking enemies.
     * @param enemies An array of enemies.
     */
    public void checkAttack(ArrayList<Enemy> enemies) {
        // loop through enemies
        for (Enemy enemy : enemies) {
            // check knife attack
            if (knifeHit) {
                if (facingRight) {
                    if (enemy.getx() > x && enemy.getx() < x + knifeRange && enemy.gety() > y - height / 2 && enemy.gety() < y + height / 2) {
                        enemy.hit(knifeDamage);
                    }
                } else {
                    if (enemy.getx() < x && enemy.getx() > x - knifeRange && enemy.gety() > y - height / 2 && enemy.gety() < y + height / 2) {
                        enemy.hit(knifeDamage);
                    }
                }
            }

            // check shooting attack
            for (Bullet bullet : bullets) {
                if (bullet.intersects(enemy)) {
                    enemy.hit(bulletDamage);
                    bullet.setHit();
                    break;
                }
            }

            // check for enemy collision
            if (intersects(enemy)) {
                hit(enemy.getDamage());
            }
        }
    }

    private void hit(int damage) {
        if (flinch) return;
        if (shield > 0) {
            shield -= damage;

        }
        else {
            health -= damage;
        }

        if (health < 0) health = 0;
        if (health == 0) dead = true;
        flinch = true;
        flinchTimer = System.nanoTime();
    }

    private void spritesChanger() throws IOException {

        // nothing on
        if (playerHotBar.get(HELMET_SLOT) == null &&
                playerHotBar.get(VEST_SLOT) == null &&
                LOOT_ON_ME != 1) {
            playerUtilities.changeSprites(frames, 1, PlayerUtilities.SET);
            LOOT_ON_ME = 1;
        }

        // only the vest
        else if (playerHotBar.get(HELMET_SLOT) == null &&
                playerHotBar.get(VEST_SLOT) != null &&
                LOOT_ON_ME != 2) {
            playerUtilities.changeSprites(frames, 2, PlayerUtilities.SET);
            LOOT_ON_ME = 2;
        }

        // only the helmet
        else if (playerHotBar.get(HELMET_SLOT) != null &&
                playerHotBar.get(VEST_SLOT) == null &&
                LOOT_ON_ME != 3) {
            playerUtilities.changeSprites(frames, 3, PlayerUtilities.SET);
            LOOT_ON_ME = 3;
        }

        //helmet & vest
        else if (playerHotBar.get(HELMET_SLOT) != null &&
                playerHotBar.get(VEST_SLOT) != null &&
                LOOT_ON_ME != 4) {
            playerUtilities.changeSprites(frames, 4, PlayerUtilities.SET);
            LOOT_ON_ME = 4;
        }
    }

    /**
     * Updates the player.
     * Position, collision, action, animation, clothes and etc.
     */
    public void update() {

        try {
            spritesChanger();
        } catch (IOException e) {
            System.err.println(Arrays.toString(e.getStackTrace()));
        }

        getNextPos();
        checkCollision();
        setPosition(xtemp, ytemp);

        // check attack has stopped
        if (currentAction == ATTACKING) {
            if (animation.hasPlayedOnce()) knifeHit = false;
        }

        if (currentAction == BULLET) {
            if (animation.hasPlayedOnce()) shooting = false;
        }

        if (reloading) {
            fire = maxFire;
        }

        if (shooting && currentAction != BULLET && currentAction != JUMPING) {
            fire += 1;
            if (fire > maxFire)
                fire = maxFire;
            if (fire > fireCost) {
                fire -= fireCost;
                Bullet bl = new Bullet(tileMap, facingRight);
                bl.setPosition(x, y - 11);
                bullets.add(bl);

                if (playerHotBar.get(GUN_SLOT) != null && playerHotBar.get(GUN_SLOT).unit.equals("m4") &&
                        !soundBoard.getFile().equals("m4"))
                    soundBoard = new SoundBoard("m4");
                if (playerHotBar.get(GUN_SLOT) == null && !soundBoard.getFile().equals("m4"))
                    soundBoard = new SoundBoard("pistol");
                soundBoard.play();
            }
        }

        // update bullets
        for (int i = 0; i < bullets.size(); i++) {
            bullets.get(i).update();
            if (bullets.get(i).removing()) {
                bullets.remove(i);
                i--;
            }
        }

        // check done flinching
        if (flinch) {
            long elapsed = (System.nanoTime() - flinchTimer) / 1000000;
            if (elapsed > 1000) {
                flinch = false;
            }
        }

        if (knifeHit) {
            if (currentAction != ATTACKING) {
                currentAction = ATTACKING;
                animation.setFrames(frames.get(ATTACKING));
                animation.setDelay(80);
                if (!soundBoard.getFile().equals("knife"))
                    soundBoard = new SoundBoard("knife");
                soundBoard.play();
            }
        }

        if (shooting) {
            if (currentAction != BULLET) {
                currentAction = BULLET;
                animation.setFrames(frames.get(BULLET));
                animation.setDelay(30);
            }
        }
        if (dy < 0) {
            if (currentAction != JUMPING) {
                currentAction = JUMPING;
                animation.setFrames(frames.get(IDLE));
                animation.setDelay(300);
            }
        }

        if (left || right) {
            if (currentAction != WALKING && !shooting && !knifeHit) {
                currentAction = WALKING;
                animation.setFrames(frames.get(WALKING));
                animation.setDelay(100);
            }
        } else {
            if (currentAction != IDLE && !shooting && !knifeHit) {
                currentAction = IDLE;
                animation.setFrames(frames.get(IDLE));
                animation.setDelay(300);
            }
        }
        //update only frames
        animation.update();
    }


    private void getNextPos() {

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
        } else if (dx > 0) {
            dx -= stopSpeed;
            if (dx < 0)
                dx = 0;
        } else if (dx < 0) {
            dx += stopSpeed;
            if (dx > 0)
                dx = 0;
        }

        if (jumping && !falling) {
            dy = jumpStart;
            falling = true;
        }

        if (falling) {
            dy += fallSpeed;
            if (dy > 0) jumping = false;
            if (dy < 0 && !jumping) dy += stopJumpSpeed;
            if (dy > maxFallSpeed) dy = maxFallSpeed;
        }
    }

    /**
     * Draws the player.
     * @param g The Graphics class, used to draw.
     */
    public void draw(Graphics2D g) {
        setMapPosition();

        for (Bullet bullet : bullets)
            bullet.draw(g);

        // draw player flinching
        if (flinch) {
            long elapsed = (System.nanoTime() - flinchTimer) / 1000000;
            if (elapsed / 100 % 2 == 0) return;
        }
        super.draw(g);
    }

    /**
     * Set the max health of the player.
     * @param maxHealth The int, representing max health.
     */
    public void setMaxHealth(int maxHealth) {
        this.maxHealth = maxHealth;
    }

    /**
     * Set the fire rate of the player.
     * @param fire The int, representing fire rate.
     */
    public void setFire(int fire) {
        this.fire = fire;
    }

    /**
     * Set the max fire rate of the player.
     * @param maxFire The int, representing max fire rate.
     */
    public void setMaxFire(int maxFire) {
        this.maxFire = maxFire;
    }

    /**
     * Set the one shot cost of the player.
     * @param fireCost The int, representing one shot cost.
     */
    public void setFireCost(int fireCost) {
        this.fireCost = fireCost;
    }

    /**
     * Set the damage deal by knife attack of the player.
     * @param knifeDamage The int, representing knife damage.
     */
    public void setKnifeDamage(int knifeDamage) {
        this.knifeDamage = knifeDamage;
    }

    /**
     * Set the range of a knife attack of the player.
     * @param knifeRange The int, representing range.
     */
    public void setKnifeRange(int knifeRange) {
        this.knifeRange = knifeRange;
    }

    /**
     * Set the 1 shot damage of the player.
     * @param bulletDamage The int, representing damage.
     */
    public void setBulletDamage(int bulletDamage) {
        this.bulletDamage = bulletDamage;
    }

    /**
     * Set the 'dead' statement of the player.
     * @param death The boolean, representing 'is dead' statement.
     */
    public void setDead(boolean death) {
        dead = death;
    }

    /**
     * Set the health of the player.
     * @param health The int, representing health.
     */
    public void setHealth(int health) {
        this.health = health;
    }

    /**
     * Set the shield of the player.
     * @param shield The int, representing shields.
     */
    public void setShield(int shield) {
        this.shield = shield;
    }

    /**
     * Gets the cost of 1 shot.
     * @return fireCost The int, representing cost of shot.
     */
    public int getFireCost() {
        return fireCost;
    }

    /**
     * Gets the knife damage.
     * @return knifeDamage The int, representing knife damage.
     */
    public int getKnifeDamage() {
        return knifeDamage;
    }

    /**
     * Gets the knife attack range.
     * @return knifeRange The int, representing range.
     */
    public int getKnifeRange() {
        return knifeRange;
    }

    /**
     * Gets the damage of bullet.
     * @return bulletDamage The int, representing damage.
     */
    public int getBulletDamage() {
        return bulletDamage;
    }

    /**
     * Gets the max fire rate.
     * @return maxFire The int, representing rate.
     */
    public int getMaxFire() {
        return maxFire;
    }

    /**
     * Gets the inventory of the player.
     * @return inventory The Inventory object.
     */
    public Inventory getInventory() {
        return inventory;
    }

    /**
     * Gets the health.
     * @return health The int, representing health.
     */
    public int getHealth() {
        return health;
    }

    /**
     * Gets the max health.
     * @return maxHealth The int, representing max health.
     */
    public int getMaxHealth() {
        return maxHealth;
    }

    /**
     * Gets the shields.
     * @return shield The int, representing shields.
     */
    public int getShield() {
        return shield;
    }

    /**
     * Gets the fire rate.
     * @return fire The int, representing fire rate.
     */
    public int getFire() {
        return fire;
    }

    /**
     * Gets the sex of the player.
     * @return sex The String, representing sex.
     */
    public String getSex() {
        return sex;
    }

    /**
     * Gets the hotbar of the player.
     * @return playerHotBar The ArrayList, representing player's hotbar.
     */
    public ArrayList<Loot> getPlayerHotBar() {
        return playerHotBar;
    }

    /**
     * Gets the player's utils.
     * @return playerUtilities The PlayerUtilities object.
     */
    public PlayerUtilities getPlayerUtilities() {
        return playerUtilities;
    }

    /**
     * Gets current animation frames of the object.
     * @return frames The ArrayList, representing current animation frames.
     */
    public ArrayList<BufferedImage[]> getFrames() {
        return frames;
    }

    /**
     * Gets 'is dead' player's statement.
     * @return dead The boolean, representing 'is dead' statement.
     */
    public boolean isDead() {
        return dead;
    }

}
