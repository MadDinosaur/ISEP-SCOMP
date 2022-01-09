public class OrderHandler extends Thread{
    private OrderQueue orderQueue;

    public OrderHandler(OrderQueue orderQueue) {
        this.orderQueue = orderQueue;
    }

    @Override
    public void run() {
        //Handles an order every 5 seconds
        while (!Thread.interrupted()) {

            Order order = orderQueue.pollOrder();
            System.out.printf("Handling %s...\n", order);

            try {
                sleep(5000);
            } catch (InterruptedException e) {
                System.out.println("Order handler shutting down...");
                return;
            }
        }
        System.out.println("Order handler shutting down...");
        return;
    }
}
