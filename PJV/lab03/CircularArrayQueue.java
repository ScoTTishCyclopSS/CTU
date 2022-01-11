package cz.cvut.fel.pjv;

/**
 * Implementation of the {@link Queue} backed by fixed size array.
 *  * @author timusfed
 */
public class CircularArrayQueue implements Queue {

    public String[] arr;
    public int front;
    public int rear;
    public int size;
    /**
     * Creates the queue with capacity set to the value of 5.
     */
    public CircularArrayQueue() {
        this.arr = new String[5];
        this.front = this.rear = this.size = 0;
    }


    /**
     * Creates the queue with given {@code capacity}. The capacity represents maximal number of elements that the
     * queue is able to store.
     * @param capacity of the queue
     */
    public CircularArrayQueue(int capacity) {
        arr = new String[capacity];
        this.front = this.rear = this.size = 0;
    }

    @Override
    public int size() { //ok;
        return this.size;
    }

    @Override
    public boolean isEmpty() { //ok
        return size() == 0;
    }

    @Override
    public boolean isFull() { //ok
        return size() == this.arr.length;
    }

    @Override
    public boolean enqueue(String obj) { //push - ok
        if(isFull() || obj == null)
            return false;
        this.arr[this.rear] = obj;
        this.rear = (this.rear + 1) % this.arr.length;
        this.size++;
        return true;
    }

    @Override
    public String dequeue() { //pop - ok
        String head = null;
        if (isEmpty() || size() == 0)
            return head;
        head = this.arr[this.front];
        this.front = (this.front + 1) % this.arr.length;
        this.size--;
        return head;
    }

    @Override
    public void printAllElements() {
        if (!isEmpty())
            for (int i = front; i < this.front + size(); i++)
                System.out.println(this.arr[i]);
    }
}
