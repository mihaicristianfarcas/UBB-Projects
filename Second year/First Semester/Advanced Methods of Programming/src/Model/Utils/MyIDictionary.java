package Model.Utils;

import Model.Values.Value;

import java.util.Collection;
import java.util.Map;

public interface MyIDictionary<K, V> {
    void update(K key, V value);
    V lookup(K key);
    boolean isDefined(K key);
    void remove(K key);
    Iterable<K> getAllKeys();
    Collection<Value> getAllValues();
    void clear();
    int size();
    boolean isEmpty();
    String toString();
    Map<K, V> getContent();
}
