package GameState;

import Coop.Client;
import Coop.Server;
import Coop.ServerTest;
import Entity.Player;
import Sound.SoundBoard;
import TileMap.BackgroundTile;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyEvent;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.net.Socket;
import java.util.Arrays;
import java.util.Objects;

import static GameState.GameStateManager.LEVEL_STATE;
import static javax.swing.JOptionPane.PLAIN_MESSAGE;

/**
 * Represents the first state of the game: menu.
 * @author timusfed
 * @author shirover
 */
public class MenuState extends GameState {

    //player
    private final SoundBoard soundBoard;

    //background
    private final BackgroundTile bg;

    //menu
    private int currentChoice = 0;
    private final String[] menu;

    //fonts
    private final Color titleColor;
    private final Font titleFont;
    private final Font menuFont;

    //counters for the dynamic bg
    private int countBg = 1;
    private int countMs = 0;

    //pointer
    private BufferedImage[] beans;

    /**
     * Creates the MenuState object.
     * Sets the GameStateManager, Background, Title, Fonts, Pointer, Menu, SoundBoard, Availability.
     * @param gsm Represents the GameStateManager.
     */
    public MenuState(GameStateManager gsm) {

        super();
        this.gsm = gsm;
        bg = new BackgroundTile("/Backgrounds/1.gif", 0.5);

        titleColor = new Color(255, 255, 255);
        titleFont = new Font("Century Gothic", Font.PLAIN, 80);

        this.menu = new String[] {"Play", "Options", "Quit"};
        menuFont = new Font("Arial", Font.PLAIN, 30);

        try {
            BufferedImage menuBeans = ImageIO.read(
                    Objects.requireNonNull(getClass().
                            getResourceAsStream("/Loot/supply/for_menu/beans_menu.gif")));
            beans = new BufferedImage[1];
            beans[0] = menuBeans.getSubimage( 0, 0, 120, 120);
        } catch (IOException e) {
            System.err.println(Arrays.toString(e.getStackTrace()));
        }

        soundBoard = new SoundBoard("vltava");
        soundBoard.play();

        isAvailable = true;
    }

    @Override
    public void update() {
        // dynamic bg
        countMs++;
        if (countBg == 16)
            countBg = 1;
        if (countMs == 13) {
            bg.update("/menuBg/" + countBg + ".gif");
            countBg++;
            countMs = 0;
        }
    }

    @Override
    public void draw(Graphics2D g) {
        // draw bg
        bg.draw(g);

        // draw title
        g.setColor(titleColor);
        g.setFont(titleFont);
        g.drawString("The Afterlife", 80, 150);

        //draw menu options
        g.setFont(menuFont);
        for (int i = 0; i < menu.length; i++) {
            if (i == currentChoice) {
                g.setColor(Color.RED);
                g.drawImage(beans[0], 160, 480 + i * 40, null);
            }
            else
                g.setColor(Color.WHITE);
            g.drawString(menu[i], 80, 550 + i * 40);
        }
    }

    @Override
    public void keyPressed_(int keyCode) {
        if (keyCode == KeyEvent.VK_ENTER) {
            try {
                select();
            } catch (IOException e) {
                System.err.println(Arrays.toString(e.getStackTrace()));
            }
        }
        if (keyCode == KeyEvent.VK_UP) {
            currentChoice--;
            if (currentChoice == -1) {
                currentChoice = menu.length - 1;
            }
        }
        if (keyCode == KeyEvent.VK_DOWN) {
            currentChoice++;
            if (currentChoice == menu.length) {
                currentChoice = 0;
            }
        }
    }

    private void select() throws IOException {
        if (currentChoice == 0) {
            gsm.setState(LEVEL_STATE);
            soundBoard.close();
        }

        if (currentChoice == 1)
            JOptionPane.showMessageDialog(null, settingsPane(), "Game settings", PLAIN_MESSAGE);

        if (currentChoice == 2)
            System.exit(0);
    }

    private JPanel genderPanel() throws IOException {
        //new panel
        JPanel panel = new JPanel();

        //new buttons
        JButton manBtn = new JButton();
        JButton womanBtn = new JButton();

        //icons for buttons
        String womanPath = "/player/woman/woman_idle/woman_idle1.gif";
        String manPath = "/player/man/man_idle/man_idle1.gif";
        ImageIcon womanIcon = new ImageIcon(
                ImageIO.read(Objects.requireNonNull(getClass()
                        .getResourceAsStream(womanPath)))
                        .getSubimage(0,0,120,120)
        );
        ImageIcon manIcon = new ImageIcon(
                ImageIO.read(Objects.requireNonNull(getClass()
                        .getResourceAsStream(manPath)))
                        .getSubimage(0,0,120,120)
        );

        //adding functions to the buttons
        manBtn.addActionListener(e -> gsm.setPlayerSex("man"));
        womanBtn.addActionListener(e -> gsm.setPlayerSex("woman"));

        //adding buttons to the panel
        manBtn.setIcon(manIcon);
        womanBtn.setIcon(womanIcon);
        panel.add(manBtn);
        panel.add(womanBtn);

        return panel;
    }

    private JPanel clientPanel() {
        JPanel clientPane = new JPanel();

        JLabel status = new JLabel();

        JLabel sv = new JLabel();
        sv.setText("Start server: ");

        JLabel ip = new JLabel();
        ip.setText("Server IP: ");

        JTextField ipTextFiled = new JTextField();
        ipTextFiled.setText("localhost");

        JButton serverStartBtn = new JButton();
        JButton connectBtn = new JButton();

        //server stuff
        serverStartBtn.setText("Start server!");
        serverStartBtn.addActionListener(actionEvent -> {
            Server server = new Server(gsm);
            server.start();
            gsm.setServer(server);
            GameStateManager.SERVER_IS_ONLINE = true;

            Client client = new Client(ipTextFiled.getText());
            client.start();
            gsm.setClient(client);
            GameStateManager.IM_CONNECTED = true;


            status.setText("Server started & Client connected");
            serverStartBtn.setEnabled(false);
            connectBtn.setEnabled(false);
            gsm.getGameStates().set(LEVEL_STATE, new LevelStateCoop(gsm));
        });

        //client stuff
        connectBtn.setText("Join");
        connectBtn.addActionListener(actionEvent -> {
            Client client = new Client(ipTextFiled.getText());
            gsm.setClient(client);
            client.start();
            GameStateManager.IM_CONNECTED = true;

            status.setText("Client connected");
            serverStartBtn.setEnabled(false);
            connectBtn.setEnabled(false);
            gsm.getGameStates().set(LEVEL_STATE, new LevelStateCoop(gsm));
        });

        //add everything
        clientPane.add(ip);
        clientPane.add(ipTextFiled);
        clientPane.add(connectBtn);
        clientPane.add(sv);
        clientPane.add(serverStartBtn);
        clientPane.add(status);

        return clientPane;
    }

    private JTabbedPane settingsPane() throws IOException {
        JTabbedPane settings = new JTabbedPane();
        settings.addTab("Player", genderPanel());
        settings.addTab("COOP", clientPanel());
        return settings;
    }

    // doesn't use in there
    @Override
    public void init() {

    }

    @Override
    public void keyReleased_(int keyCode) {

    }

    @Override
    public void setPlayer(Player player) {

    }

    @Override
    public void keyPressed_(Socket client, int keyCode) {
    }

    @Override
    public void keyReleased_(Socket client, int keyCode) {

    }
}
