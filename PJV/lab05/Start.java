package cz.cvut.fel.pjv;

public class Start {

    public static void main(String[] args) {
        int a = 7;
        int[] values = new int[a];

        for (int i = 0; i < a; i++ )
            values[i] = i + 1;

        Tree tree = new TreeImpl();
        tree.setTree(values);
        System.out.println(tree.toString());
    }
}
