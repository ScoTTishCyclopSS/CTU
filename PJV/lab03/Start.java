package cz.cvut.fel.pjv;

import java.awt.*;

/**
 * @author Jirka Sebek
 */
public class Start {

    public static void main(String[] args) {

         
        /* homework */
        Queue queue = new CircularArrayQueue(10);
        queue.enqueue("Starkiller");
        queue.enqueue("C-3PO");
        queue.enqueue("Jabba the Hutt");
        queue.enqueue("HK-47");
        queue.enqueue("Darth Nihilus"); //Cool mask, tho
        queue.enqueue("Count Dooku"); //Joda is disappointed in you, Dooku...
        System.out.println("size: " + queue.size()); // size = 6
        System.out.println("Value dequeued from CircularArrayQueue: " + queue.dequeue()); // remove "Starkiller"
        System.out.println("printing all elements: ");
        queue.printAllElements(); // C-3PO -> Count Dooku
        queue.enqueue("Jango Fett"); // add "Jango Fett"
        System.out.println("size: " + queue.size());
    }
}
