package cz.cvut.fel.pjv;

public class Test {
    
    public void start() {
        String password = "abcdaaaddb";
        BruteForceAttacker2 attacker = new BruteForceAttacker2();
        attacker.init(new char[]{'a', 'b', 'c', 'd'}, password);
        
        System.out.println("Trying to break password...");
        attacker.breakPassword(password.length());
        
        if (attacker.isOpened()) {
            System.out.println("[VAULT] opened, password is " + password);
        } else {
            System.out.println("[VAULT] is still closed");
        }
    }
}