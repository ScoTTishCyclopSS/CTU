package GameState;

import Entity.Player;

import static GameState.GameState.TILE_SIZE;

/**
 * Represents the state changer, when the player reach 'special' position on a map.
 * Looks like the asymptote on X coordinate.
 * @author timusfed
 */
public class Teleport {

    private final int x;

    /**
     * Creates the Teleport object.
     * @param x The int number, representing asymptote on X coordinate.
     */
    public Teleport(int x) {
        this.x = x * TILE_SIZE;
    }

    /**
     * 'Teleports' the player to another location or state, depends on developer choice.
     * @param player The player object to transfer.
     * @param state The index of state to teleport.
     * @param gsm TRepresents GameStateManager.
     */
    public void initTeleportation(Player player, int state, GameStateManager gsm) {
        int oldState = gsm.getCurrentStateCode();
        gsm.setState(state);
        gsm.getGameStates().set(oldState, null);
    }

    /**
     * Gets X position of the Teleport.
     * @return x The coordinate itself.
     */
    public int getX() {
        return x;
    }
}
