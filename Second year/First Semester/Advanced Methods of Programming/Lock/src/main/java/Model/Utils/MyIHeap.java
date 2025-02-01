package Model.Utils;

import java.util.Map;

public interface MyIHeap<V> {
    void update(Integer address, V value);
    V lookup(Integer address);
    boolean isDefined(Integer address);
    void remove(Integer address);
    Iterable<Integer> getAllAddresses();
    Iterable<V> getAllValues();
    void clear();
    int size();
    Integer allocate(V val);
    void setContent(Map<Integer, V> integerValueMap);
    Map<Integer, V> getContent();
    String toString();
}
