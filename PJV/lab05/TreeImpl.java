/*
 * author: @timusfed
 */
 
package cz.cvut.fel.pjv;
 
public class TreeImpl implements Tree {
 
    Node root;
    int[] values;
 
    @Override
    public void setTree(int[] values) {
        this.values = values;
        if (values.length == 0)
            root = null;
        else
            root = createNode(0, values.length - 1); // the first index -> the last one,
        // like "create node on this part of the array"
    }
 
    /*
     * So yea, my idea with copying parts of the array was actually garbage.
     * Instead of it I should work with indexes, in that way I will lower the memory load.
     *  + It doesn't need to be cast as a parameter. Only indexes.
     */
    public Node createNode(int left, int right) { // [1, 2, 3, 4, 5, 6, 7]
        if (left == right) // if the "array.length" is 1
            return new NodeImpl(values[left]);
 
        int center = findCenterIndex(left, right); // 3 = [ 4 ]
        NodeImpl node = new NodeImpl(values[center]);
 
        if (left <= center - 1)
            node.left = createNode(left, center - 1); // 0 -> 2 = [1, 2, 3], lower the right index on each step
        if (center + 1 <= right)
            node.right = createNode(center + 1, right); // 4 -> 6 = [5, 6, 7] reduce the left index on each step
 
        return node;
    }
 
    /*
     * For me this part was kind of a challenge, my first couple of tries didn't work with small arrays
     * Creating universal finder, I mean
     */
    public static int findCenterIndex(int left, int right) { // find center index
        if ((left + right) % 2 != 0)
            return ((left + right) / 2) + 1; // exp. (0 + 1) / 2 = 0.5 => +1 = 1.5 => (int) 1, done
        return (left + right) / 2; // exp. (0 + 2) / 2 = 1, done
    }
 
 
    // now is useless... Before the toString method, I was using that to create tree string while recursion is working
    // but sadly it should be created via toString method
    public String repeater(String str, int count) {
        if (count == 0)
            return "";
        StringBuilder strBuilder = new StringBuilder(str);
        for (int i = 0; i < count; i++)
            strBuilder.append(" ");
        str = strBuilder.toString();
        return str;
    }
 
    @Override
    public Node getRoot() {
        return root;
    }
 
    @Override
    public String toString() {
        return printNode(this.root, 0);
    }
 
    // adding depth if only we go deeper inside the tree: left, right etc.
    public String printNode(Node node, int depth) {
        String nodeStr = "";
        if (node != null) {
            for (int i = 0; i < depth; i++) {
                nodeStr += " ";
            }
            nodeStr += "- " + node.getValue() + "\n";
            nodeStr += printNode(node.getLeft(), depth + 1);
            nodeStr += printNode(node.getRight(), depth + 1);
        }
        return nodeStr;
    }
}