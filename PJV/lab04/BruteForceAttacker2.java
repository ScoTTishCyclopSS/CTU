/*
 * author: @timusfed
 */

package cz.cvut.fel.pjv;

public class BruteForceAttacker2 extends Thief{

    @Override
    public void breakPassword(int sizeOfPassword) {
        char[] alphabet = getCharacters(); //all possible chars
        breaker(alphabet, "", sizeOfPassword);
    }

    public void breaker(char[] alphabet, String previous, int i) 
    {
        if (isOpened())
            return;
        if (i == 0)
        {
            tryOpen(previous.toCharArray());
            return;
        }
        for (char c : alphabet) {
            String next = previous + c;
            System.out.println(next);
            breaker(alphabet, next, i - 1);
        }
    }
}