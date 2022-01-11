package Entity.Loot;

import Entity.Player;
import TileMap.TileMap;
import org.json.simple.parser.ParseException;
import org.junit.Assert;
import org.junit.Test;

import java.io.IOException;

import static GameState.GameState.TILE_SIZE;

/**
 * Makes the test of the action perform on a player by beans.
 * @author timusfed
 */
public class BeansTest {

    @Test
    public void testBeans() throws IOException, ParseException {
        TileMap tm = new TileMap(TILE_SIZE);
        Player player = new Player(tm, "man", null);
        Loot beans = new Loot(tm, Loot.SUPPLY_LOOT, "beans");
        beans.action(player);
        Assert.assertEquals(2, player.getHealth());
    }
}
