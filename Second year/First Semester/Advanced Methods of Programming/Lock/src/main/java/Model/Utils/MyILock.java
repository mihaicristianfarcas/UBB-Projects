package Model.Utils;

import java.util.Map;

public interface MyILock {

    boolean containsKey(int key);

    void update(int key, int value);

    int lookup(int key);

    int getFreeLocation();

    Map<Integer, Integer> getContent();
}
