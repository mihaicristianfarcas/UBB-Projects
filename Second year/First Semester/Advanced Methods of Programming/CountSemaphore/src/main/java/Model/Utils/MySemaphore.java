package Model.Utils;

import java.util.List;

public class MySemaphore {
    private int permits;
    private List<Integer> acquired;

    public MySemaphore(int permits, List<Integer> acquired) {
        this.permits = permits;
        this.acquired = acquired;
    }

    public int getPermits() {
        return permits;
    }

    public List<Integer> getAcquired() {
        return acquired;
    }

    public void setPermits(int permits) {
        this.permits = permits;
    }

    public void setAcquired(List<Integer> acquired) {
        this.acquired = acquired;
    }

    @Override
    public String toString() {
        return "(" + Integer.valueOf(permits).toString() + ", " + acquired.toString() + ")";
    }
}
