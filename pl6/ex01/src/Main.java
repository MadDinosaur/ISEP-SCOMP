import java.util.ArrayList;
import java.util.List;

public class Main {
    static final int HANDLERS = 4;
    static final int TAKERS = 2;
    static final Thread[] handlers = new OrderHandler[HANDLERS];
    static final Thread[] takers = new OrderTaker[TAKERS];

    public static void main(String[] args) throws InterruptedException {
        //Creates a shared OrderQueue
        OrderQueue queue = new OrderQueue();

        //Initializes OrderTaker and OrderHandler threads
        for(int i = 0; i < TAKERS; i++) {
            OrderTaker orderTaker = new OrderTaker(queue);
            takers[i] = orderTaker;
            takers[i].start();
        }

        for(int i = 0; i < HANDLERS; i++) {
            OrderHandler orderHandler = new OrderHandler(queue);
            handlers[i] = orderHandler;
            handlers[i].start();
        }

        //Main thread sleeps for 15 seconds
        Thread.sleep(10000);

        //Interrupts all active threads and shuts down
        for(Thread taker: takers) {
            System.out.println("Interrupting " + taker);
            taker.interrupt();
            taker.join();
        }

        for(Thread handler: handlers) {
            System.out.println("Interrupting " + handler);
            handler.interrupt();
            handler.join();
        }

        System.out.println("Taking no more orders. Shutting down...");
    }
}
