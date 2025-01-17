package Model.Utils;

import java.util.HashMap;
import java.util.Map;

public class MyHeap<V> implements MyIHeap<V> {
    private java.util.HashMap<Integer, V> heap;
    private static int newAddress = 1;

    public MyHeap() {
        heap = new java.util.HashMap<>();
    }

    @Override
    public void update(Integer address, V value) {
        heap.put(address, value);
    }

    @Override
    public V lookup(Integer address) {
        return heap.get(address);
    }

    @Override
    public boolean isDefined(Integer address) {
        return heap.containsKey(address);
    }

    @Override
    public void remove(Integer address) {
        heap.remove(address);
    }

    @Override
    public Iterable<Integer> getAllAddresses() {
        return heap.keySet();
    }

    @Override
    public Iterable<V> getAllValues() {
        return heap.values();
    }

    @Override
    public void clear() {
        heap.clear();
    }

    @Override
    public int size() {
        return heap.size();
    }

    @Override
    public Integer allocate(V val) {
        heap.put(newAddress, val);
        newAddress++;
        return newAddress - 1;
    }

    @Override
    public void setContent(Map<Integer, V> integerValueMap) {
        this.heap = (HashMap<Integer, V>) integerValueMap;
    }

    @Override
    public Map<Integer, V> getContent() {
        return heap;
    }

    @Override
    public String toString() {
        StringBuilder str = new StringBuilder();
        for (Integer key : heap.keySet()) {
            str.append(key.toString()).append(" -> ").append(heap.get(key).toString()).append("\n");
        }
        return str.toString();
    }
}