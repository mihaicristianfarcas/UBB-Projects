package Model.Utils;

import java.util.List;

public class MySemaphore {
    private int permits;
    private List<Integer> acquired;

    public MySemaphore(int permits, List<Integer> acquired) {
        this.permits = permits;
        this.acquired = acquired;
    }

    public synchronized int getPermits() {
        return permits;
    }

    public synchronized List<Integer> getAcquired() {
        return acquired;
    }

    public synchronized void setPermits(int permits) {
        this.permits = permits;
    }

    public synchronized void setAcquired(List<Integer> acquired) {
        this.acquired = acquired;
    }

    @Override
    public synchronized String toString() {
        return "(" + Integer.valueOf(permits).toString() + ", " + acquired.toString() + ")";
    }
}
