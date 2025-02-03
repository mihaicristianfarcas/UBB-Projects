package Model.Utils;

import java.util.HashMap;
import java.util.Map;

public interface MyISemaphoreTable {
    void put(int key, MySemaphore value);

    MySemaphore get(int key);

    boolean containsKey(int key);

    int getFreeAddress();

    Map<Integer, MySemaphore> getSemaphoreTable();

    void setSemaphoreTable(HashMap<Integer, MySemaphore> newSemaphoreTable);

    public Map<Integer, MySemaphore> getContent();
}