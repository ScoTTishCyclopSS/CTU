package main;

import javax.swing.*;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintStream;

class Game {

    /*
     * The main method for starting up the game.
     * Creating new window with the GamePanel.
     */
    public static void main(String[] args) {
        JFrame window = new JFrame();
        initLogger();
        window.setContentPane(new GamePanel());
        window.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        window.setResizable(false);
        window.pack();
        window.setVisible(true);
    }

    private static void initLogger()
    {
        File file = new File("log.txt");
        FileOutputStream fos = null;
        try {
            fos = new FileOutputStream(file);
        } catch (FileNotFoundException e) {
            System.err.println(e.getMessage());
        }
        PrintStream ps = new PrintStream(fos);
        //System.setErr(new PrintStream(ps));
    }
}
