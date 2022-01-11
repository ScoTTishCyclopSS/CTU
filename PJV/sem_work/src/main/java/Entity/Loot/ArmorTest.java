package Entity.Loot;

import Entity.Player;
import TileMap.TileMap;
import org.json.simple.parser.ParseException;
import org.junit.Assert;
import org.junit.Test;

import java.io.IOException;

import static GameState.GameState.TILE_SIZE;

/**
 * Makes the test of the action perform on a player by different types of armor.
 * @author shirover
 */
public class ArmorTest {
    @Test
    public void testHelmet() throws IOException, ParseException {
        TileMap tm = new TileMap(TILE_SIZE);
        Player player = new Player(tm, "man", null);
        Loot helmet = new Loot(tm, Loot.ARMOR_LOOT, "helmet");
        helmet.action(player);
        Assert.assertEquals(2, player.getShield());
    }

    @Test
    public void testVest() throws IOException, ParseException {
        TileMap tm = new TileMap(TILE_SIZE);
        Player player = new Player(tm, "man", null);
        Loot vest = new Loot(tm, Loot.ARMOR_LOOT, "vest");
        vest.action(player);
        Assert.assertEquals(3, player.getShield());
    }
}
