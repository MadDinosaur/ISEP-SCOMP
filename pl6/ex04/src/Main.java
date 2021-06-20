import java.util.Random;

public class Main {
    static final int TAXIS = 10;
    static final int REQUESTS = 20;
    static final Thread[] requests = new TaxiRequest[REQUESTS];
    static final Thread[] taxis = new Taxi[TAXIS];

    public static void main(String[] args) throws InterruptedException {
        //Creates a shared Taxi Central
        TaxiCentral central = new TaxiCentral();

        //Initializes TaxiRequest and Taxi threads with random values
        for(int i = 0; i < TAXIS; i++) {
            int maxSeats = (new Random().nextInt(10) + 1);

            Taxi taxi = new Taxi(central, maxSeats);
            taxis[i] = taxi;
            taxis[i].start();
        }

        for(int i = 0; i < REQUESTS; i++) {
            int maxSeats = (new Random().nextInt(10) + 1);
            String address = "address";

            TaxiRequest request = new TaxiRequest(central, maxSeats, address);
            requests[i] = request;
            requests[i].start();
        }

        //Waits for active threads
        for(Thread request: requests) {
            request.join();
        }
        for(Thread taxi: taxis) {
            taxi.join();
        }
    }
}
