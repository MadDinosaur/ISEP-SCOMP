public class TicketSeller extends Thread {
    private TicketStore store;

    public TicketSeller(TicketStore store) {
        this.store = store;
    }

    @Override
    public void run() {
        while(!Thread.interrupted()) {
            try {
                Ticket ticket = store.sellTicket();
                System.out.printf("Seller %d sold %s\n", this.getId(), ticket);

                sleep(2000);
            } catch (InterruptedException e) {
                System.out.println("Seller shutting down...");
                return;
            }
        }
        System.out.println("Seller shutting down...");
        return;
    }
}
