import java.util.LinkedList;

public class TaxiCentral {
    private LinkedList<TaxiRequest> requests = new LinkedList<>();

    public synchronized TaxiRequest pollRequest() {
        while (!hasRequests()) {
            System.out.println("There are no requests to accept. Waiting...");
            try {
                wait();
            } catch (InterruptedException e) {
                System.out.println("Waiting list interrupted");
            }
        }
        return requests.poll();
    }

    public synchronized void addRequest(TaxiRequest request) {
        if (requests.add(request)) {
            System.out.printf("Added %s to waiting list.\n", request);
            notify();
        } else {
            System.out.printf("Failed to add %s to waiting list.\n", request);
        }
    }

    public boolean hasRequests() {
        return !requests.isEmpty();
    }
}
