package Sound;


import javax.sound.sampled.*;
import java.io.IOException;
import java.util.Arrays;

/**
 * Represents the sound player tool for the game.
 * @author timusfed
 */
public class SoundBoard {

    private Clip clip;
    private final String file;

    /**
     * Creates the new SoundBoard with the specified sound, or music.
     * @param s The string, representing file's name.
     */
    public SoundBoard(String s) {
        file = s;
        try {
            AudioInputStream incomeSound = AudioSystem.getAudioInputStream(
                    getClass().getResourceAsStream("/sfx/" + s + ".mp3"));
            AudioFormat baseFormat = incomeSound.getFormat();
            AudioFormat decodeFormat = new AudioFormat(
                    AudioFormat.Encoding.PCM_SIGNED,
                    baseFormat.getSampleRate(),
                    16,
                    baseFormat.getChannels(),
                    baseFormat.getChannels() * 2,
                    baseFormat.getSampleRate(),
                    false);
            AudioInputStream decoded = AudioSystem.getAudioInputStream(decodeFormat, incomeSound);
            clip = AudioSystem.getClip();
            clip.open(decoded);
        } catch (UnsupportedAudioFileException | IOException | LineUnavailableException e) {
            System.err.println(Arrays.toString(e.getStackTrace()));
        }
    }

    /**
     * Plays the sound or music.
     */
    public void play() {
        if (clip == null)
            return;
        if (clip.isRunning())
            clip.stop();
        clip.setFramePosition(0);
        clip.start();
    }

    /**
     * Closing the opened sound board.
     */
    public void close() {
        if (clip.isRunning())
            clip.stop();
        clip.close();
    }

    /**
     * Gets the sound or music file's name.
     * @return file The string, representing file's name.
     */
    public String getFile() {
        return file;
    }
}
