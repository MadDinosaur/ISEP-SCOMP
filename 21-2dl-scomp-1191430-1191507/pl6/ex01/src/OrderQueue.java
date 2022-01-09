import java.util.LinkedList;

public class OrderQueue {
    private LinkedList<Order> orders;
    private int orderNumber;

    public OrderQueue() {
        this.orderNumber = 1;
        this.orders = new LinkedList<>();
    }

    public synchronized Order addOrder() {
        Order order = new Order(orderNumber);

        if (orders.add(order)) {
            System.out.printf("Added %s to queue.\n", order);
            orderNumber++;
            notify();
            return order;
        } else {
            System.out.printf("Failed to add %s to queue.\n", order);
            return null;
        }
    }

    public synchronized Order pollOrder() {
        while (!hasOrders()) {
            System.out.println("There are no orders to handle");
            try {
                wait();
            } catch (InterruptedException e) {
                System.out.println("Queue interrupted");
            }
        }
        return orders.poll();
    }

    public boolean hasOrders() {
        return !orders.isEmpty();
    }
}
