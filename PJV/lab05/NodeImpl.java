/*
 * author: @timusfed
 */
 
package cz.cvut.fel.pjv;
 
public class NodeImpl implements Node{
 
    Node left, right;
    int value;
 
    public NodeImpl(int value) {
        this.value = value;
        this.left = null;
        this.right = null;
    }
 
    @Override
    public Node getLeft() {
        return this.left;
    }
 
    @Override
    public Node getRight() {
        return this.right;
    }
 
    @Override
    public int getValue() {
        return this.value;
    }
}