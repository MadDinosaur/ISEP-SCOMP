public class Ticket {
    private int number;

    public Ticket(int number) {
        this.number = number;
    }

    @Override
    public String toString() {
        return "Ticket no. " + number;
    }
}
