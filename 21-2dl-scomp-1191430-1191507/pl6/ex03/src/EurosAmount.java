public class EurosAmount{
    private double amount;

    public EurosAmount(double amount) {
        this.amount = amount;
    }

    public void add(EurosAmount amount) {
        this.amount += amount.amount;
    }

    public void subtract(EurosAmount amount) {
        if (this.amount - amount.amount < 0)
            throw new UnsupportedOperationException("Insufficient balance!");

        this.amount -= amount.amount;
    }

    @Override
    public String toString() {
        return String.format("%.2f€", amount);
    }

    public int compareTo(double amount) {
        return Double.compare(this.amount, amount);
    }
}
