package Entity.Loot;

import TileMap.TileMap;
import org.junit.Assert;
import org.junit.Test;

import static GameState.GameState.TILE_SIZE;

/**
 * Makes the test of the filling the container with loot.
 * @author timusfed
 * @author shirover
 */
public class ContainerTest {

    @Test
    public void putTheLoot() {
        TileMap tm = new TileMap(TILE_SIZE);
        Loot unit = new Loot(tm, Loot.SUPPLY_LOOT, "beans");
        LootContainer lootContainer = new LootContainer(tm, 2, "box");
        lootContainer.putLoot(unit);
        Assert.assertEquals(1, lootContainer.getLoot().size());
    }

    @Test
    public void containerDestroy() {
        TileMap tm = new TileMap(TILE_SIZE);
        LootContainer lootContainer = new LootContainer(tm, 2, "box");
        lootContainer.hit(16);
        Assert.assertTrue(lootContainer.isDestroyed());
    }
}
