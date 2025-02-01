package Model.Utils;

import java.util.HashMap;
import java.util.Map;

public class MyLock implements MyILock {
    private HashMap<Integer, Integer> lockTable;
    int newFreeLocation = 0;

    public MyLock() {
        this.lockTable = new HashMap<>();
    }

    @Override
    public synchronized int getFreeLocation() {
        newFreeLocation++;
        return newFreeLocation;
    }

    @Override
    public synchronized void update(int key, int value) {
        lockTable.put(key, value);
    }

    @Override
    public boolean containsKey(int key) {
        return lockTable.containsKey(key);
    }

    @Override
    public int lookup(int key) {
        return lockTable.get(key);
    }

    public Map<Integer, Integer> getContent() {
        return lockTable;
    }

    @Override
    public String toString() {
        StringBuilder str = new StringBuilder();
        for (Integer key : lockTable.keySet()) {
            str.append(key.toString()).append(" -> ").append(lockTable.get(key).toString()).append("\n");
        }
        return str.toString();
    }
}
