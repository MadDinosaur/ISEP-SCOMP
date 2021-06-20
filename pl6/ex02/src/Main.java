public class Main {
    static final int SELLERS = 5;
    static final int TICKETS = 20;
    static final Thread[] sellers = new TicketSeller[SELLERS];

    public static void main(String[] args) throws InterruptedException {
        //Creates a shared TicketStore
        TicketStore store = new TicketStore(TICKETS);

        //Initializes TicketSeller threads
        for(int i = 0; i < SELLERS; i++) {
            TicketSeller ticketSeller = new TicketSeller(store);
            sellers[i] = ticketSeller;
            sellers[i].start();
        }
    }

    public static void interruptAll() throws InterruptedException {
        System.out.println("All tickets have been sold!");

        //Interrupts all active threads and shuts down
        for(Thread seller: sellers) {
            seller.interrupt();
            seller.join();
        }
        System.out.println("No longer selling tickets. Shutting down...");
        System.exit(0);
    }
}
