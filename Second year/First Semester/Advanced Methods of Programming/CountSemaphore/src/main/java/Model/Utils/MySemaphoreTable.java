package Model.Utils;

import java.util.HashMap;
import java.util.Map;

public class MySemaphoreTable implements MyISemaphoreTable {
    private HashMap<Integer, MySemaphore> semaphoreTable;
    private int freeLocation = 0;

    public MySemaphoreTable() {
        this.semaphoreTable = new HashMap<>();
    }

    @Override
    public synchronized void put(int key, MySemaphore value) {
        semaphoreTable.put(key, value);
    }

    @Override
    public MySemaphore get(int key) {
        return semaphoreTable.get(key);
    }

    @Override
    public boolean containsKey(int key) {
        return semaphoreTable.containsKey(key);
    }

    @Override
    public synchronized int getFreeAddress() {
        freeLocation++;
        return freeLocation;
    }

    @Override
    public Map<Integer, MySemaphore> getSemaphoreTable() {
        return semaphoreTable;
    }

    @Override
    public void setSemaphoreTable(HashMap<Integer, MySemaphore> newSemaphoreTable) {
        this.semaphoreTable = newSemaphoreTable;
    }

    @Override
    public String toString() {
        StringBuilder str = new StringBuilder();
        for (Integer key : semaphoreTable.keySet()) {
            str.append(key.toString()).append(" -> ").append(semaphoreTable.get(key).toString()).append("\n");
        }
        return str.toString();
    }

    @Override
    public Map<Integer, MySemaphore> getContent() {
        return semaphoreTable;
    }
}