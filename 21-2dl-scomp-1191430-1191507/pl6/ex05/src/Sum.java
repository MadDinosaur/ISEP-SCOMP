import java.util.Arrays;

public class Sum implements Runnable {
    int startIndex, endIndex, sum;
    int[] array;

    /**
     * Sums a vector of integers in the interval [startIndex, endIndex[
     * @param startIndex - inclusive, the first index to cover
     * @param endIndex - exclusive, index immediately past the last index to cover
     */
    public Sum(int[] array, int startIndex, int endIndex) {
        this.startIndex = startIndex;
        this.endIndex = endIndex;
        this.array = array;
    }

    @Override
    public void run() {
        System.out.println("Thread " + Thread.currentThread().getId() + " calculating sum between index " + startIndex + " and " + endIndex + "...");
        sum = Arrays.stream(array, startIndex, endIndex).sum();
        System.out.printf("Sum [%d, %d[ = %d\n", startIndex, endIndex, sum);
    }
}
