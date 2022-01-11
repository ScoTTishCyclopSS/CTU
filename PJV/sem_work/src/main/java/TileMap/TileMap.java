package TileMap;

import main.GamePanel;

import java.awt.*;
import java.awt.image.*;

import java.io.*;
import java.util.Arrays;
import java.util.Objects;
import javax.imageio.ImageIO;

/**
 * Represents the tool to organize tiles inside the gaming space.
 * @author timusfed
 * @author shirover
 */
public class TileMap {

    //map
    private int[][] map;
    private final int tileSize;
    private int numRows;
    private int numCols;
    private double tween;

    private Tile[][] tiles;

    //drawing
    private int rowOffset;
    private int colOffset;
    private final int numRowsToDraw;
    private final int numColsToDraw;

    //position
    private int x;
    private int y;

    //bounds
    private int xmin;
    private int ymin;
    private int xmax;
    private int ymax;

    /**
     * Creates the new TileMap object.
     * Sets the tile size, tween.
     * Calculates and the sets the numRowsToDraw, numColsToDraw (+ 1, to avoid the unloading tiles or appearance).
     * @param tileSize The int, representing preferred size of one tile.
     */
    public TileMap(int tileSize) {
        this.tileSize = tileSize;
        numRowsToDraw = GamePanel.HEIGHT / tileSize;
        numColsToDraw = GamePanel.WIDTH / tileSize + 1; //for "no loading" tiles
        tween = 0.07; //smoothly camera
    }

    /**
     * Loads tiles to the tiles array, sort them by category.
     * @param tilesPath The String, representing file's path.
     */
    public void loadTiles(String tilesPath) {
        try {
            // loading file with tiles
            BufferedImage tileset = ImageIO.read(Objects.requireNonNull(
                    getClass().getResourceAsStream(tilesPath)));
            int tilesAlong = tileset.getWidth() / tileSize; // number of tiles along
            int tilesAcross = tileset.getHeight() / tileSize; // number of tiles across
            tiles = new Tile[tilesAcross][tilesAlong];

            BufferedImage subImg;
            for (int col = 0; col < tilesAlong; col++) {
                //creating new tiles according to the type, adding them to the array
                subImg = tileset.getSubimage(col * tileSize, 0, tileSize, tileSize);
                tiles[Tile.NORMAL][col] = new Tile(subImg, Tile.NORMAL);
                subImg = tileset.getSubimage(col * tileSize, tileSize, tileSize, tileSize);
                tiles[Tile.BLOCKED][col] = new Tile(subImg, Tile.BLOCKED);
            }
        } catch (IOException e) {
            System.err.println(Arrays.toString(e.getStackTrace()));
        }
    }

    /**
     * Loads the map file, reads it, keeps the information for the future use in draw.
     * @param mapPath The String, representing file's path.
     */
    public void loadMap(String mapPath) {
        try {
            // loading map via text file
            InputStream in = getClass().getResourceAsStream(mapPath);
            BufferedReader br = new BufferedReader(new InputStreamReader(Objects.requireNonNull(in)));

            // loading of primary information about the map
            numRows = Integer.parseInt(br.readLine());
            numCols = Integer.parseInt(br.readLine());
            map = new int[numRows][numCols];

            int mapWidth = numCols * tileSize;
            int mapHeight = numRows * tileSize;

            // creating work area
            xmin = GamePanel.WIDTH - mapWidth;
            ymin = GamePanel.HEIGHT - mapHeight;
            xmax = 0;
            ymax = 0;

            // saving map to the array
            for (int row = 0; row < numRows; row++) {
                String[] mapPoint = br.readLine().split("\\s+");
                for (int col = 0; col < numCols; col++)
                    map[row][col] = Integer.parseInt(mapPoint[col]);
            }

        } catch (IOException e) {
            System.err.println(Arrays.toString(e.getStackTrace()));
        }
    }

    /**
     * Gets type of the tile by coordinates.
     * @param row The int, representing the row of the tile map.
     * @param col The int, representing the column of the tile map.
     * @return The int, representing the type of the tile.
     */
    public int getType(int row, int col) {
        if (map[row][col] == 0)
            return 0;
        char[] rc = String.valueOf(map[row][col]).toCharArray();
        // -1 is to avoid the 0 using
        return Integer.parseInt(String.valueOf(rc[0])) - 1;
    }

    public void setTween(double d) { tween = d; }

    /**
     * Sets the position of the tile map on a screen.
     * @param x The double, representing the X coordinate.
     * @param y The double, representing the Y coordinate.
     */
    public void setPosition(double x, double y) {
        this.x += (x - this.x) * tween;
        this.y += (y - this.y) * tween;

        // if coordinates were entered outside the work area
        fixBounds();

        colOffset = -this.x / tileSize;
        rowOffset = -this.y / tileSize;
    }

    private void fixBounds() {
        if (x < xmin) x = xmin;
        if (x > xmax) x = xmax;
        if (y < ymin) y = ymin;
        if (y > ymax) y = ymax;
    }

    /**
     * Draws the tile map to the screen directly, using the map.
     * @param g The Graphics class, used to draw.
     */
    public void draw(Graphics2D g) {
        for (int row = rowOffset; row < rowOffset + numRowsToDraw; row++) {

            if (row >= numRows) break;

            for (int col = colOffset; col < colOffset + numColsToDraw; col++) {

                if (col >= numCols) break;

                if (map[row][col] == 0) continue;

                // getting tiles from array
                char[] rc = String.valueOf(map[row][col]).toCharArray();
                int row_tile = Integer.parseInt(String.valueOf(rc[0])) - 1;
                int clm_tile = Integer.parseInt(String.valueOf(rc[1]));

                g.drawImage(tiles[row_tile][clm_tile].getImage(),
                        x + col * tileSize,
                        y + row * tileSize,
                        null
                );
            }
        }
    }

    /**
     * Gets the X coordinate of the tile map.
     * @return The int, representing the X coordinate.
     */
    public double getx() {
        return x;
    }

    /**
     * Gets the Y coordinate of the tile map.
     * @return The int, representing the Y coordinate.
     */
    public double gety() {
        return y;
    }
}
