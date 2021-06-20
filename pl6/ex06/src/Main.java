import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.concurrent.*;

public class Main {
    final static int THREADS = 5;
    final static int PARTITIONS = 10;
    final static int ARRAY_SIZE = 100;

    public static void main(String[] args) throws ExecutionException, InterruptedException {
        //Creates array and initializes it with random values
        int[] array = new int[ARRAY_SIZE];
        initializeArray(array);
        //Creates array to store partial sums
        List<Future<Integer>> sums = new ArrayList<>();

        //Initialize Executor with fixed thread count
        ExecutorService executor = Executors.newFixedThreadPool(THREADS);

        //Run Sum threads using Executor
        for (int i = 0; i < PARTITIONS; i++) {
            int interval = ARRAY_SIZE/PARTITIONS;

            int start = i * interval;
            int end = start + interval;

            Future<Integer> sum = executor.submit(new Sum(array, start, end));
            sums.add(sum);
        }

        //Waits for current threads to finish execution
        executor.shutdown();
        executor.awaitTermination(10, TimeUnit.SECONDS);

        //Prints all partial sums - since Future is blocking, printing here will not delay threads from being created
        System.out.printf("Partial sums = [ ");
        for(Future<Integer> sum: sums)
            System.out.printf(" %d ", sum.get());
        System.out.printf("]\n");

        System.exit(0);
    }

    private static void initializeArray(int[] array) {
        for(int i = 0; i < array.length; i++)
            array[i] = (new Random()).nextInt(50);
    }
}
