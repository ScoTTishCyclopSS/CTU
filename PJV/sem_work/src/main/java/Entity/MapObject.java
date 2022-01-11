package Entity;

import GameState.GameState;
import TileMap.TileMap;
import TileMap.Tile;

import java.awt.*;

/**
 * Represents any physic object in game.
 * @author timusfed
 * @author shirover
 */
public abstract class MapObject {

    // tile stuff
    protected TileMap tileMap;
    protected int tileSize;
    protected double xmap;
    protected double ymap;

    // position and vector
    protected double x;
    protected double y;
    protected double dx;
    protected double dy;

    // dimensions
    protected int width;
    protected int height;

    // collision box
    protected int cwidth;
    protected int cheight;

    // collision
    protected int currRow;
    protected int currCol;
    protected double xdest;
    protected double ydest;
    protected double xtemp;
    protected double ytemp;
    protected boolean topLeft;
    protected boolean topRight;
    protected boolean bottomLeft;
    protected boolean bottomRight;

    // animation
    protected AnimationStuff animation;
    protected int currentAction;
    protected int previousAction;
    protected boolean facingRight;

    // movement
    protected boolean left;
    protected boolean right;
    protected boolean jumping;
    protected boolean falling;
    protected boolean shooting;
    protected boolean reloading;
    protected boolean knifeHit;

    // movement attributes
    protected double moveSpeed;
    protected double maxSpeed;
    protected double stopSpeed;
    protected double fallSpeed;
    protected double maxFallSpeed;
    protected double jumpStart;
    protected double stopJumpSpeed;

    /**
     * Creates the new map object.
     * Sets the tile size.
     * @param tm Tile map object.
     */
    public MapObject(TileMap tm) {
        this.tileMap = tm;
        this.tileSize = GameState.TILE_SIZE;
    }

    /**
     * Checks if the object interests with any other object.
     * @param o Map object.
     * @return r1.intersects(r2) The boolean, representing 'is interests' statement.
     */
    public boolean intersects(MapObject o) {
        Rectangle r1 = getRectangle();
        Rectangle r2 = o.getRectangle();
        return r1.intersects(r2);
    }

    /**
     * Creates the 'abstract' rectangle from the object coords.
     * @return new Rectangle New Rectangle object.
     */
    public Rectangle getRectangle() {
        return new Rectangle((int) x - cwidth, (int) y - cheight, cwidth, cheight);
    }

    /**
     * Sets the position of the object to the specified.
     * @param x The int number, representing X coordinate.
     * @param y The int number, representing Y coordinate.
     */
    public void setPosition(double x, double y) {
        if (x < 100) x = 100;
        if (y < 100) y = 100;
        this.x = x;
        this.y = y;
    }

    /**
     * Set position of the map, accordion to map object.
     */
    public void setMapPosition() {
        this.xmap = tileMap.getx();
        this.ymap = tileMap.gety();
    }

    /**
     * Checks if the map object collies with any others.
     */
    public void checkCollision() {

        // current row and current column (where is a player)
        this.currCol = (int) this.x / this.tileSize;
        this.currRow = (int) this.y / this.tileSize;

        // where to go (vector)
        this.xdest = this.x + this.dx;
        this.ydest = this.y + this.dy;

        // final destination, based on calculations
        this.xtemp = this.x;
        this.ytemp = this.y;

        // left corner
        calculateCorners(x, ydest);

        // up
        if (dy < 0) {
            if (!topLeft && !topRight) ytemp += dy;
        }

        // down
        if (dy > 0) {
            if (bottomLeft || bottomRight) falling = false;
            else ytemp += dy;
        }

        // right corner
        calculateCorners(xdest, y);

        if (dx < 0) {
            if (!topLeft && !bottomLeft) {
                xtemp += dx;
            } else {
                dx = 0;
                xtemp = currCol * tileSize + cwidth / 2;
            }
        }
        if (dx > 0) {
            if (!topRight && !bottomRight) {
                xtemp += dx;
            } else {
                dx = 0;
                xtemp = (currCol + 1) * tileSize - cwidth / 2;
            }
        }

        if (!falling) {
            calculateCorners(x, ydest + 1);
            if (!bottomRight && !bottomLeft) falling = true;
        }
    }

    private void calculateCorners(double x, double y) {

        int leftTile = (int) (x - this.cwidth / 2) / this.tileSize;
        int rightTile = (int) (x + this.cwidth / 2) / this.tileSize;
        int topTile = (int) (y - this.cheight / 2) / this.tileSize;
        int bottomTile = (int) (y + this.cheight / 2) / this.tileSize;

        int tl = tileMap.getType(topTile, leftTile);
        int tr = tileMap.getType(topTile, rightTile);
        int bl = tileMap.getType(bottomTile, leftTile);
        int br = tileMap.getType(bottomTile, rightTile);

        topLeft = tl == Tile.BLOCKED;
        topRight = tr == Tile.BLOCKED;
        bottomLeft = bl == Tile.BLOCKED;
        bottomRight = br == Tile.BLOCKED;
    }

    public void draw(Graphics2D g) {

        if (facingRight) {
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

    /**
     * Sets the X coordinate of the map object.
     * @param x The int number, representing X coordinate.
     */
    public void setX(double x) {
        this.x = x;
    }

    /**
     * Sets the Y coordinate of the map object.
     * @param y The int number, representing Y coordinate.
     */
    public void setY(double y) {
        this.y = y;
    }

    /**
     * Sets the dx coordinate of the map object. (Part of the vector)
     * @param dx The int number, representing dx coordinate.
     */
    public void setDx(double dx) {
        this.dx = dx;
    }

    /**
     * Sets the dy coordinate of the map object. (Part of the vector)
     * @param dy The int number, representing dy coordinate.
     */
    public void setDy(double dy) {
        this.dy = dy;
    }

    /**
     * Sets the width of the map object.
     * @param width The int number, representing width.
     */
    public void setWidth(int width) {
        this.width = width;
    }

    /**
     * Sets the height of the map object.
     * @param height The int number, representing height.
     */
    public void setHeight(int height) {
        this.height = height;
    }

    /**
     * Sets the cwidth of the map object.
     * @param cwidth The int number, representing cwidth.
     */
    public void setCwidth(int cwidth) {
        this.cwidth = cwidth;
    }

    /**
     * Sets the cheight of the map object.
     * @param cheight The int number, representing cheight.
     */
    public void setCheight(int cheight) {
        this.cheight = cheight;
    }

    /**
     * Sets the moveSpeed of the map object.
     * @param moveSpeed The double number, representing moveSpeed.
     */
    public void setMoveSpeed(double moveSpeed) {
        this.moveSpeed = moveSpeed;
    }

    /**
     * Sets the maxSpeed of the map object.
     * @param maxSpeed The double number, representing maxSpeed.
     */
    public void setMaxSpeed(double maxSpeed) {
        this.maxSpeed = maxSpeed;
    }

    /**
     * Sets the stopSpeed of the map object.
     * @param stopSpeed The double number, representing stopSpeed.
     */
    public void setStopSpeed(double stopSpeed) {
        this.stopSpeed = stopSpeed;
    }

    /**
     * Sets the fallSpeed of the map object.
     * @param fallSpeed The double number, representing fallSpeed.
     */
    public void setFallSpeed(double fallSpeed) {
        this.fallSpeed = fallSpeed;
    }

    /**
     * Sets the maxFallSpeed of the map object.
     * @param maxFallSpeed The double number, representing maxFallSpeed.
     */
    public void setMaxFallSpeed(double maxFallSpeed) {
        this.maxFallSpeed = maxFallSpeed;
    }

    /**
     * Sets the jumpStart of the map object.
     * @param jumpStart The double number, representing jumpStart.
     */
    public void setJumpStart(double jumpStart) {
        this.jumpStart = jumpStart;
    }

    /**
     * Sets the stopJumpSpeed of the map object.
     * @param stopJumpSpeed The double number, representing stopJumpSpeed.
     */
    public void setStopJumpSpeed(double stopJumpSpeed) {
        this.stopJumpSpeed = stopJumpSpeed;
    }

    /**
     * Sets the 'facingRight' statement of the map object.
     * @param facingRight The boolean, representing 'facingRight' statement.
     */
    public void setFacingRight(boolean facingRight) {
        this.facingRight = facingRight;
    }

    /**
     * Sets the left direction of the map object.
     * @param left The boolean, representing left direction statement.
     */
    public void setLeft(boolean left) {
        this.left = left;
    }

    /**
     * Sets the right direction of the map object.
     * @param right  The boolean, representing right direction statement.
     */
    public void setRight(boolean right) {
        this.right = right;
    }

    /**
     * Sets the jumping of the map object.
     * @param jumping The boolean, representing jumping statement.
     */
    public void setJumping(boolean jumping) {
        this.jumping = jumping;
    }

    /**
     * Sets the shooting of the map object.
     * @param shooting The boolean, representing shooting statement.
     */
    public void setShooting(boolean shooting) {
        this.shooting = shooting;
    }

    /**
     * Sets the reloading of the map object.
     * @param reloading The boolean, representing reloading statement.
     */
    public void setReload(boolean reloading) {
        this.reloading = reloading;
    }

    /**
     * Sets the knifeHit of the map object.
     * @param knifeHit The boolean, representing knifeHit statement.
     */
    public void setKnifeHit(boolean knifeHit) {
        this.knifeHit = knifeHit;
    }

    /**
     * Gets the X coordinate of the map object.
     * @return x The int, representing the X coordinate.
     */
    public int getx() {
        return (int) this.x;
    }

    /**
     * Gets the Y coordinate of the map object.
     * @return y The int, representing the Y coordinate.
     */
    public int gety() {
        return (int) this.y;
    }

    /**
     * Gets the dX coordinate of the map object.
     * @return dx The int, representing the dX coordinate.
     */
    public double getDx() {
        return dx;
    }

    /**
     * Gets the dy coordinate of the map object.
     * @return dy The int, representing the dy coordinate.
     */
    public double getDy() {
        return dy;
    }

    /**
     * Gets the width of the map object.
     * @return width The int, representing the width.
     */
    public int getWidth() {
        return width;
    }

    /**
     * Gets the height of the map object.
     * @return height The int, representing the height.
     */
    public int getHeight() {
        return height;
    }

    /**
     * Gets the cwidth of the map object.
     * @return cwidth The int, representing the cwidth.
     */
    public int getCwidth() {
        return cwidth;
    }

    /**
     * Gets the cheight of the map object.
     * @return cheight The int, representing the cheight.
     */
    public int getCheight() {
        return cheight;
    }

    /**
     * Gets the moveSpeed of the map object.
     * @return moveSpeed The double, representing the moveSpeed.
     */
    public double getMoveSpeed() {
        return moveSpeed;
    }

    /**
     * Gets the maxSpeed of the map object.
     * @return maxSpeed The double, representing the maxSpeed.
     */
    public double getMaxSpeed() {
        return maxSpeed;
    }

    /**
     * Gets the stopSpeed of the map object.
     * @return stopSpeed The double, representing the stopSpeed.
     */
    public double getStopSpeed() {
        return stopSpeed;
    }

    /**
     * Gets the fallSpeed of the map object.
     * @return fallSpeed The double, representing the fallSpeed.
     */
    public double getFallSpeed() {
        return fallSpeed;
    }

    /**
     * Gets the maxFallSpeed of the map object.
     * @return maxFallSpeed The double, representing the maxFallSpeed.
     */
    public double getMaxFallSpeed() {
        return maxFallSpeed;
    }

    /**
     * Gets the jumpStart of the map object.
     * @return jumpStart The double, representing the jumpStart.
     */
    public double getJumpStart() {
        return jumpStart;
    }

    /**
     * Gets the stopJumpSpeed of the map object.
     * @return stopJumpSpeed The double, representing the stopJumpSpeed.
     */
    public double getStopJumpSpeed() {
        return stopJumpSpeed;
    }

    /**
     * Gets the 'facingRight' statement of the map object.
     * @return facingRight The boolean, representing the 'facingRight' statement.
     */
    public boolean getFacingRight() {
        return facingRight;
    }

    /**
     * Gets the 'left' statement of the map object.
     * @return left The boolean, representing the 'left' statement.
     */
    public boolean getLeft() {
        return left;
    }

    /**
     * Gets the 'right' statement of the map object.
     * @return right The boolean, representing the 'right' statement.
     */
    public boolean getRight() {
        return right;
    }

    /**
     * Gets the currentAction of the map object.
     * @return currentAction The int, representing current action of the player.
     */
    public int getCurrentAction() {
        return currentAction;
    }
}
