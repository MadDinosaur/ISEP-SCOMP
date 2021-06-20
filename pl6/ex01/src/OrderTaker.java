public class OrderTaker extends Thread {
    private OrderQueue orderQueue;

    public OrderTaker(OrderQueue orderQueue) {
        this.orderQueue = orderQueue;
    }

    @Override
    public void run() {
        //Takes an order every 5 seconds
        while (!Thread.interrupted()) {
            orderQueue.addOrder();

            try {
                sleep(2000);
            } catch (InterruptedException e) {
                System.out.println("Order taker shutting down...");
                return;
            }
        }
        System.out.println("Order taker shutting down...");
        return;
    }
}
