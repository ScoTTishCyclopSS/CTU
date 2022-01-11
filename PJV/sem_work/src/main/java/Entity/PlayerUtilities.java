package Entity;

import Entity.AnimationStuff;
import Entity.Player;
import GameState.GameStateManager;
import GameState.LevelState;
import GameState.LevelStateCoop;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Objects;

import static GameState.GameStateManager.LEVEL_STATE;

/**
 * Represent the collection of the tool, useful to load or to sav player information.
 * @author timusfed
 */
public class PlayerUtilities {

    /**
     * Represents command constant to frames changing operation as 'load'.
     */
    public static final int LOAD = 0;
    /**
     * Represents command constant to frames changing operation as 'set'.
     */
    public static final int SET = 1;

    private final Player player;

    /**
     * Creates new object of PlayerUtilities.
     * Sets the Player.
     * @param player Player object.
     */
    public PlayerUtilities(Player player) {
        this.player = player;
    }

    /**
     * Loads player's params from file.
     * @param file The load operation depends on param: 'null' new player, 'file' player with specified params.
     * @throws IOException Exception on read or non-existing files.
     * @throws ParseException Exception on a parse process.
     */
    public void loadPlayer(File file) throws IOException, ParseException {
        InputStream path = getClass().getResourceAsStream("/player/player.json");

        if (file != null)
            path = new FileInputStream(file);

        InputStreamReader in = new InputStreamReader(path, StandardCharsets.UTF_8);

        JSONParser parser = new JSONParser();
        JSONObject object = (JSONObject) parser.parse(in);

        player.setX(((Number) object.get("x")).doubleValue());
        player.setY(((Number) object.get("y")).doubleValue());
        player.setDx(((Number) object.get("dx")).doubleValue());
        player.setDx(((Number) object.get("dy")).doubleValue());
        player.setWidth(((Number) object.get("width")).intValue());
        player.setHeight(((Number) object.get("height")).intValue());
        player.setCwidth(((Number) object.get("cwidth")).intValue());
        player.setCheight(((Number) object.get("cheight")).intValue());
        player.setMoveSpeed(((Number) object.get("moveSpeed")).doubleValue());
        player.setMaxSpeed(((Number) object.get("maxSpeed")).doubleValue());
        player.setStopSpeed(((Number) object.get("stopSpeed")).doubleValue());
        player.setFallSpeed(((Number) object.get("fallSpeed")).doubleValue());
        player.setMaxFallSpeed(((Number) object.get("maxFallSpeed")).doubleValue());
        player.setJumpStart(((Number) object.get("jumpStart")).doubleValue());
        player.setStopJumpSpeed(((Number) object.get("stopJumpSpeed")).doubleValue());
        player.setFacingRight((boolean) object.get("facingRight"));
        player.setHealth(((Number) object.get("health")).intValue());
        player.setMaxHealth(((Number) object.get("maxHealth")).intValue());
        player.setShield(((Number) object.get("shield")).intValue());
        player.setFire(((Number) object.get("fire")).intValue());
        player.setMaxFire(((Number) object.get("maxFire")).intValue());
        player.setFireCost(((Number) object.get("fireCost")).intValue());
        player.setKnifeDamage(((Number) object.get("knifeDamage")).intValue());
        player.setKnifeRange(((Number) object.get("knifeRange")).intValue());
        player.setBulletDamage(((Number) object.get("bulletDamage")).intValue());
        player.setLeft((boolean) object.get("left"));
        player.setRight((boolean) object.get("right"));
        player.currentAction = (((Number) object.get("currentAction")).intValue());

        in.close();
    }

    /**
     * Saves player params to the file.
     * @param i The load operation depends on param: '0' single player saving, 'counter' CO-OP saving.
     */
    public void savePlayer(int i) {
        try {
            File file;
            if (i == 0) {
                file = new File("players_save/" + "player_" + i + ".json");
                file.getParentFile().mkdirs(); // Will create parent directories if not exists
                file.createNewFile();
            }
            else {
                file = new File("players_online/" + "player_" + i + ".json");
                file.getParentFile().mkdirs(); // Will create parent directories if not exists
                file.createNewFile();
            }
            // Creates an OutputStreamWriter
            OutputStreamWriter writer = new OutputStreamWriter(new FileOutputStream(file));

            writer.write("{\n");
            writer.write("\t\"x\":" + player.getx() + ",\n");
            writer.write("\t\"y\":" + player.gety() + ",\n");
            writer.write("\t\"dx\":" + player.getDx() + ",\n");
            writer.write("\t\"dy\":" + player.getDy() + ",\n");
            writer.write("\t\"width\":" + player.getWidth() + ",\n");
            writer.write("\t\"height\":" + player.getHeight() + ",\n");
            writer.write("\t\"cwidth\":" + player.getCwidth() + ",\n");
            writer.write("\t\"cheight\":" + player.getCheight() + ",\n");
            writer.write("\t\"moveSpeed\":" + player.getMoveSpeed() + ",\n");
            writer.write("\t\"maxSpeed\":" + player.getMaxSpeed() + ",\n");
            writer.write("\t\"stopSpeed\":" + player.getStopSpeed() + ",\n");
            writer.write("\t\"fallSpeed\":" + player.getFallSpeed() + ",\n");
            writer.write("\t\"maxFallSpeed\":" + player.getMaxFallSpeed() + ",\n");
            writer.write("\t\"jumpStart\":" + player.getJumpStart() + ",\n");
            writer.write("\t\"stopJumpSpeed\":" + player.getStopJumpSpeed() + ",\n");
            writer.write("\t\"facingRight\":" + player.getFacingRight() + ",\n");
            writer.write("\t\"health\":" + player.getHeight() + ",\n");
            writer.write("\t\"maxHealth\":" + player.getMaxHealth() + ",\n");
            writer.write("\t\"shield\":" + player.getShield() + ",\n");
            writer.write("\t\"fire\":" + player.getFire() + ",\n");
            writer.write("\t\"maxFire\":" + player.getMaxFire() + ",\n");
            writer.write("\t\"fireCost\":" + player.getFireCost() + ",\n");
            writer.write("\t\"knifeDamage\":" + player.getKnifeDamage() + ",\n");
            writer.write("\t\"knifeRange\":" + player.getKnifeRange() + ",\n");
            writer.write("\t\"bulletDamage\":" + player.getBulletDamage() + ",\n");
            writer.write("\t\"right\":" + player.getRight() + ",\n");
            writer.write("\t\"left\":" + player.getLeft() + ",\n");
            writer.write("\t\"currentAction\":" + player.getCurrentAction() + "\n");
            writer.write("}\n");

            writer.close();

        } catch (IOException e) {
            System.err.println(Arrays.toString(e.getStackTrace()));
        }
    }

    /**
     * Changes sprites on player's animation.
     * Depends on
     * @param frames An ArrayList, representing frames of the player.
     * @param p The animation sprites depend on param: LOOT_ON_ME (Player).
     * @param command The changing operation depends on param: '0' LOAD, '1' SET.
     * @throws IOException Exception on reading or non-existing files.
     */
    public void changeSprites(ArrayList<BufferedImage[]> frames, int p, int command) throws IOException {
        String sex = player.getSex();
        String gunP = "pistol";
        if (player.getPlayerHotBar().get(Player.GUN_SLOT) != null)
            gunP = "m4";

        // load idle sprite [0]
        BufferedImage framesSheetIdle = ImageIO.read(Objects.requireNonNull(getClass().getResourceAsStream(
                "/player/" + sex + "/" + sex + "_idle/" + sex + "_idle" + p + ".gif")));
        BufferedImage[] id = new BufferedImage[framesSheetIdle.getWidth() / player.getWidth()];
        for (int i = 0; i < framesSheetIdle.getWidth() / player.getWidth(); i++)
            id[i] = framesSheetIdle.getSubimage(i * player.getWidth(), 0, player.getWidth(), player.getHeight());
        if (command == LOAD)
            frames.add(id);
        else if (command == SET)
            frames.set(0, id);

        // load walking sprite [1]
        BufferedImage framesSheetWalk = ImageIO.read(Objects.requireNonNull(getClass().getResourceAsStream(
                "/player/" + sex + "/" + sex + "_run/" + sex + "_run_" + gunP + "/" + sex + "_run" + p + ".gif")));
        BufferedImage[] wlk = new BufferedImage[framesSheetWalk.getWidth() / player.getWidth()];
        for (int i = 0; i < framesSheetWalk.getWidth() / player.getWidth(); i++)
            wlk[i] = framesSheetWalk.getSubimage(i * player.getWidth(), 0, player.getWidth(), player.getHeight());
        if (command == LOAD)
            frames.add(wlk);
        else if (command == SET)
            frames.set(1, wlk);

        // load knife hitting sprite [2]
        BufferedImage framesSheetKnife = ImageIO.read(Objects.requireNonNull(getClass().getResourceAsStream(
                "/player/" + sex + "/" + sex + "_attack/" + sex + "_knife" + p + ".gif")));
        BufferedImage[] knife = new BufferedImage[framesSheetKnife.getWidth() / player.getWidth()];
        for (int i = 0; i < framesSheetKnife.getWidth() / player.getWidth(); i++)
            knife[i] = framesSheetKnife.getSubimage(i * player.getWidth(), 0, player.getWidth(), player.getHeight());
        if (command == LOAD)
            frames.add(knife);
        else if (command == SET)
            frames.set(2, knife);

        // load shooting sprite [3]
        BufferedImage framesSheetShoot = ImageIO.read(Objects.requireNonNull(getClass().getResourceAsStream(
                "/player/" + sex + "/" + sex + "_run/" + sex + "_run_" + gunP + "/" + sex + "_run" + p + ".gif")));
        BufferedImage[] shoot = new BufferedImage[framesSheetShoot.getWidth() / player.getWidth()];
        for (int i = 0; i < framesSheetShoot.getWidth() / player.getWidth(); i++)
            shoot[i] = framesSheetShoot.getSubimage(i * player.getWidth(), 0, player.getWidth(), player.getHeight());
        if (command == LOAD)
            frames.add(shoot);
        else if (command == SET)
            frames.set(3, shoot);
    }

    /**
     * Creates new death dialog on player's death.
     * @param frame JFrame to put th death panel
     * @param gsm Represents GameStateManager
     * @return panel The working death dialog as panel object
     */
    public JPanel deathPanel(JFrame frame, GameStateManager gsm) {
        JPanel panel = new JPanel();
        JButton spawn = new JButton();
        JButton menu = new JButton();

        spawn.setText("Spawn");
        menu.setText("Menu");
        panel.setSize(new Dimension(150, 150));

        spawn.addActionListener(e -> {
            try {
                loadPlayer(null);
            } catch (IOException | ParseException ioException) {
                System.err.println(Arrays.toString(ioException.getStackTrace()));
            }
            player.setDead(false);
            player.setPosition(120, 400);
            frame.setVisible(false);
        });
        menu.addActionListener(e -> {
            try {
                loadPlayer(null);
            } catch (IOException | ParseException ioException) {
                System.err.println(Arrays.toString(ioException.getStackTrace()));
            }
            player.setDead(false);
            player.setPosition(120, 400);
            gsm.setState(GameStateManager.MENU_STATE);
            gsm.getGameStates().set(LEVEL_STATE, new LevelState(gsm, "level"));
            frame.setVisible(false);
        });

        panel.add(menu);
        panel.add(spawn);

        return panel;
    }
}
