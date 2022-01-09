import java.util.LinkedList;
import java.util.concurrent.Semaphore;

public class TicketStore {
    private LinkedList<Ticket> tickets = new LinkedList<>();
    private Semaphore semaphore;

    public TicketStore(int numTickets) {
        //Initializes ticket store by creating N tickets with sequential numbers
        for(int i = numTickets; i > 0; i--)
            tickets.add(new Ticket(i));
        System.out.println("Added " + numTickets + " tickets to store");
        //Initializes semaphore
        semaphore = new Semaphore(1);
    }

    public Ticket sellTicket() throws InterruptedException {
        if(!hasTickets()) {
            semaphore.release();
            Main.interruptAll();
        }

        semaphore.acquire();
        Ticket ticket =  tickets.poll();
        semaphore.release();

        return ticket;
    }

    public boolean hasTickets() {
        return !tickets.isEmpty();
    }
}
