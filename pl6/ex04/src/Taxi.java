import java.util.Random;

public class Taxi extends Thread {
    TaxiCentral taxiCentral;

    int maxSeats;

    public Taxi(TaxiCentral taxiCentral, int maxSeats) {
        this.taxiCentral = taxiCentral;
        this.maxSeats = maxSeats;
    }

    @Override
    public void run() {
        try {
            sleep(new Random().nextInt(5000));
            System.out.println("Taxi " + getId() + " available for " + maxSeats + " passenger(s)");

            TaxiRequest request = taxiCentral.pollRequest();
            System.out.println("Taxi " + getId() + " received request for " + request.numPassengers + "passenger(s)");

            //If taxi cannot carry all passengers, it creates a new TaxiRequest with the remaining passengers
            if (request.numPassengers > maxSeats) {
                System.out.println("Taxi cannot carry all passengers. Sending request for the remaining passengers...");
                TaxiRequest newTaxiRequest = new TaxiRequest(taxiCentral, request.numPassengers - maxSeats, request.address);
                newTaxiRequest.start();
            }
        } catch (InterruptedException e) {
            System.out.println("Transfer was interrupted");
            return;
        }
        return;
    }
}
