package Entity;

import Entity.Loot.Loot;
import Entity.Loot.LootContainer;
import GameState.LevelState;
import Sound.SoundBoard;

import javax.swing.*;
import javax.swing.border.LineBorder;
import java.awt.*;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

/**
 * Represents the Inventory of the Player.
 * @author timusfed
 */
public class Inventory {

    private final Player player;
    private final ArrayList<Loot> playerInventory;

    /**
     * Creates the new Player inventory.
     * Sets the player.
     * Declare array, representing the inventory with loot.
     * @param player Player object.
     */
    public Inventory (Player player) {
        this.player = player;
        this.playerInventory = new ArrayList<>();
    }

    /**
     * Initialise the inventory.
     * 1. Finds available loot;
     * 2. Generates frame dialog;
     * 3. Fills up with loot all of the panels.
     * @param loot Representing the loot from the state.
     */
    public void init(ArrayList<Loot> loot) {
        //finding available loot
        ArrayList<Loot> availableLoot = new ArrayList<>();
        loot.forEach(unit -> {
            if ((unit.getx() > player.getx() - LevelState.TILE_SIZE) &&
                    (unit.getx() < player.getx() + LevelState.TILE_SIZE)) {
                availableLoot.add(unit);
            }
        });

        // generate frame dialog
        JFrame space = new JFrame();
        JPanel panelNearLoot = createPanel(new Dimension(500, 400));
            panelNearLoot.setBorder(BorderFactory.createTitledBorder("GROUND"));
        JPanel panelPlayerLoot = createPanel(new Dimension(500, 400));
            panelPlayerLoot.setBorder(BorderFactory.createTitledBorder("INVENTORY"));
        JPanel hotBar = createPanel(new Dimension(1000, 200));
            hotBar.setBorder(BorderFactory.createTitledBorder("HOTBAR"));

        space.setSize(1000, 600);
        space.setResizable(false);
        space.setVisible(true);

        space.add(panelPlayerLoot, BorderLayout.EAST);
        space.add(panelNearLoot, BorderLayout.WEST);
        space.add(hotBar, BorderLayout.SOUTH);
        space.setVisible(true);

        // creating dictionaries of <button-loot>
        Map<JButton, Loot> nearLootBtns = new HashMap<>();
        Map<JButton, Loot> playerLootBtns = new HashMap<>();
        ArrayList<JPanel> slots = new ArrayList<>();

        // adding buttons to the equal panel
        addBtnsToPanel(availableLoot, nearLootBtns, panelNearLoot);
        addBtnsToPanel(playerInventory, playerLootBtns, panelPlayerLoot);

        //adding slots to hotbar
        player.getPlayerHotBar().forEach(unit -> {
            JPanel slot = new JPanel();
            slot.setPreferredSize(new Dimension(300, 110));
            slot.setVisible(true);
            slot.setBorder(new LineBorder(Color.BLACK));
            hotBar.add(slot);
            slots.add(slot);
        });

        player.getPlayerHotBar().forEach(unit -> {
            if (unit != null) {
                JPanel slot = slots.get(unit.slot);
                JButton btn = new JButton();
                btn.setBounds(100, 5, 100, 100);
                btn.setIcon(new ImageIcon(unit.invTexture));
                btn.setPreferredSize(new Dimension(100, 100));
                btn.addActionListener(actionEvent -> actionOnHotBar(
                        nearLootBtns, playerLootBtns, slots,
                        panelNearLoot, panelPlayerLoot, hotBar,
                        btn, unit, loot));
                slot.add(btn);
            }
        });


        // add actions
        nearLootBtns.forEach((btn, unit) -> {
            btn.addActionListener(action -> actionOnNearLoot(
                            nearLootBtns, playerLootBtns, slots,
                            panelNearLoot, panelPlayerLoot, hotBar,
                            btn, unit, loot));
        });

        playerLootBtns.forEach((btn, unit) -> {
            btn.addActionListener(action ->actionOnPlayerLoot(
                    nearLootBtns, playerLootBtns, slots,
                    panelNearLoot, panelPlayerLoot, hotBar,
                    btn, unit, loot));
        });
    }

    private void actionOnNearLoot(Map<JButton, Loot> nearLootBtns,
                                  Map<JButton, Loot> playerLootBtns,
                                  ArrayList<JPanel> slots,
                                  JPanel panelNearLoot,
                                  JPanel panelPlayerLoot,
                                  JPanel hotBar,
                                  JButton btn, Loot unit,
                                  ArrayList<Loot> nearbyLoot) {
        //remove from curr arr -> put to another
        playerLootBtns.put(btn, unit);
        nearLootBtns.remove(btn, unit);
        //play sound
        SoundBoard soundBoard = new SoundBoard(unit.getType() + "_pick");
        soundBoard.play();
        //remove from the form -> put to another
        panelPlayerLoot.add(btn);
        for( ActionListener al : btn.getActionListeners() )
            btn.removeActionListener(al);
        btn.addActionListener(action ->actionOnPlayerLoot(
                nearLootBtns, playerLootBtns, slots,
                panelNearLoot, panelPlayerLoot, hotBar,
                btn, unit, nearbyLoot));
        panelNearLoot.remove(btn);
        //repaint both sides
        panelPlayerLoot.repaint();
        panelNearLoot.repaint();
        // remove from ground -> put to inventory
        nearbyLoot.remove(unit);
        playerInventory.add(unit);
    }

    private void actionOnPlayerLoot(Map<JButton, Loot> nearLootBtns,
                                    Map<JButton, Loot> playerLootBtns,
                                    ArrayList<JPanel> slots,
                                    JPanel panelNearLoot,
                                    JPanel panelPlayerLoot,
                                    JPanel hotBar,
                                    JButton btn, Loot unit,
                                    ArrayList<Loot> nearbyLoot) {
        // perform an action
        unit.action(player);
        //remove from curr arr -> put to hot bar
        if (unit.slot != -1) {
            if(unit.slot == Player.HELMET_SLOT) {
                slots.get(Player.HELMET_SLOT).add(btn);
                btn.setBounds(100, 5, 100, 100);
            }
            if(unit.slot == Player.VEST_SLOT) {
                slots.get(Player.VEST_SLOT).add(btn);
                btn.setBounds(100, 5, 100, 100);
            }
            if(unit.slot == Player.GUN_SLOT) {
                slots.get(Player.GUN_SLOT).add(btn);
                try {
                    player.getPlayerUtilities().changeSprites(player.getFrames(), Player.LOOT_ON_ME, PlayerUtilities.SET);
                } catch (IOException e) {
                    System.err.println(Arrays.toString(e.getStackTrace()));
                }
                btn.setBounds(100, 5, 100, 100);
            }

            for( ActionListener al : btn.getActionListeners() )
                btn.removeActionListener(al);

            btn.addActionListener(action -> actionOnHotBar(
                    nearLootBtns, playerLootBtns, slots,
                    panelNearLoot, panelPlayerLoot, hotBar,
                    btn, unit, nearbyLoot));
        }
        playerLootBtns.remove(btn, unit);
        //remove from curr panel and inventory
        panelPlayerLoot.remove(btn);
        playerInventory.remove(unit);
        // repaint
        panelPlayerLoot.repaint();
        hotBar.repaint();
    }

    private void  actionOnHotBar (Map<JButton, Loot> nearLootBtns,
                                  Map<JButton, Loot> playerLootBtns,
                                  ArrayList<JPanel> slots,
                                  JPanel panelNearLoot,
                                  JPanel panelPlayerLoot,
                                  JPanel hotBar,
                                  JButton btn, Loot unit,
                                  ArrayList<Loot> nearbyLoot) {
        //remove from curr arr -> put to another
        player.getPlayerHotBar().set(unit.slot, null);
        playerInventory.add(unit);
        //remove from the form -> put to another
        playerLootBtns.put(btn, unit);

        if (unit.slot == Player.HELMET_SLOT) {
            player.setShield(player.getShield() - 2);
        }

        if (unit.slot == Player.VEST_SLOT) {
            player.setShield(player.getShield() - 3);
        }

        try {
            player.getPlayerUtilities().changeSprites(player.getFrames(), Player.LOOT_ON_ME, PlayerUtilities.SET);
        } catch (IOException e) {
            System.err.println(Arrays.toString(e.getStackTrace()));
        }

        slots.get(unit.slot).remove(btn);
        panelPlayerLoot.add(btn);
        // set new action
        for( ActionListener al : btn.getActionListeners() )
            btn.removeActionListener(al);
        btn.addActionListener(action -> actionOnPlayerLoot(
                nearLootBtns, playerLootBtns, slots,
                panelNearLoot, panelPlayerLoot, hotBar,
                btn, unit, nearbyLoot));
        //repaint
        panelPlayerLoot.repaint();
        hotBar.repaint();
    }


    private void addBtnsToPanel (ArrayList<Loot> loots, Map<JButton, Loot> map, JPanel jPanel) {
        loots.forEach(unit -> {
            JButton btn = new JButton();
            if (unit != null)
                btn.setIcon(new ImageIcon(unit.invTexture));
            btn.setPreferredSize(new Dimension(100, 100));
            map.put(btn, unit);
            jPanel.add(btn);
        });
    }

    private JPanel createPanel (Dimension dim) {
        JPanel panel = new JPanel();
        panel.setPreferredSize(dim);
        panel.setVisible(true);
        return panel;
    }

    /**
     * Gets the player's inventory.
     * @return playerInventory Inventory object
     */
    public ArrayList<Loot> getPlayerInventory() {
        return playerInventory;
    }
}
