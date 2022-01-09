import java.util.Random;

public class TaxiRequest extends Thread{
    TaxiCentral taxiCentral;

    int numPassengers;
    String address;

    public TaxiRequest(TaxiCentral taxiCentral, int numPassengers, String address) {
        this.numPassengers = numPassengers;
        this.address = address;
        this.taxiCentral = taxiCentral;
    }

    @Override
    public void run() {
        try {
            sleep(new Random().nextInt(5000));
            System.out.println("Initiating a request " + getId() + " for " + numPassengers + " passengers");

            taxiCentral.addRequest(this);
        } catch (InterruptedException e) {
            System.out.println("Transfer was interrupted");
            return;
        }
        return;
    }

    @Override
    public String toString() {
        return String.format("Request no. %d Passengers: %d", getId(), numPassengers);
    }
}
