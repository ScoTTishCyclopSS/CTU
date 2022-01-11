package Entity.Enemies;

import Entity.Player;
import TileMap.TileMap;
import org.json.simple.parser.ParseException;
import org.junit.Assert;
import org.junit.Test;

import java.io.IOException;

import static GameState.GameState.TILE_SIZE;

/**
 * Makes the test of the hit performed to the zombie.
 * @author timusfed
 * @author shirover
 */
public class ZombieDeath {

    @Test
    public void zombieHitTest () {
        TileMap tm = new TileMap(TILE_SIZE);
        Zombie zombie = new Zombie(tm);
        zombie.hit(5);
        Assert.assertEquals(10, zombie.getHealth());
    }

    @Test
    public void zombieDeathTest () {
        TileMap tm = new TileMap(TILE_SIZE);
        Zombie zombie = new Zombie(tm);
        zombie.hit(15);
        Assert.assertEquals(0, zombie.getHealth());
    }
}
