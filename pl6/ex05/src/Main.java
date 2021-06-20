import java.util.Random;
import java.util.concurrent.Executor;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Main {
    final static int THREADS = 5;
    final static int PARTITIONS = 10;
    final static int ARRAY_SIZE = 100;

    public static void main(String[] args) {
        //Creates array and initializes it with random values
        int[] array = new int[ARRAY_SIZE];
        initializeArray(array);

        //Initialize Executor with fixed thread count
        ExecutorService executor = Executors.newFixedThreadPool(THREADS);

        //Run Sum threads using Executor
        for (int i = 0; i < PARTITIONS; i++) {
            int interval = ARRAY_SIZE/PARTITIONS;

            int start = i * interval;
            int end = start + interval;

            executor.execute(new Sum(array, start, end));
        }

        //Waits for current threads to finish execution
        executor.shutdown();
    }

    private static void initializeArray(int[] array) {
        for(int i = 0; i < array.length; i++)
            array[i] = (new Random()).nextInt(50);
    }
}
