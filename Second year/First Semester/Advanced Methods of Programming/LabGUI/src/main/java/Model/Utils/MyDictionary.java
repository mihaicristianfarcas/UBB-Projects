package Model.Utils;

import Model.Exceptions.InvalidKeyException;
import java.util.Collection;
import java.util.Map;

public class MyDictionary<K, V> implements MyIDictionary<K, V> {
    private java.util.HashMap<K, V> dict;

    public MyDictionary() {
        dict = new java.util.HashMap<>();
    }

    @Override
    public void update(K key, V value) throws InvalidKeyException {
        if (key == null) {
            throw new InvalidKeyException("Key cannot be null.");
        }
        dict.put(key, value);
    }

    @Override
    public V lookup(K key) throws InvalidKeyException {
        if (!isDefined(key)) {
            throw new InvalidKeyException("Key does not exist.");
        }
        return dict.get(key);
    }

    @Override
    public boolean isDefined(K key) {
        return dict.containsKey(key);
    }

    @Override
    public void remove(K key) throws InvalidKeyException {
        if (!isDefined(key)) {
            throw new InvalidKeyException("Key does not exist.");
        }
        dict.remove(key);
    }

    @Override
    public Iterable<K> getAllKeys() {
        return dict.keySet();
    }

    @Override
    public Collection<V> getAllValues() {
        return dict.values();
    }

    @Override
    public void clear() {
        dict.clear();
    }

    @Override
    public int size() {
        return dict.size();
    }

    @Override
    public boolean isEmpty() {
        return dict.isEmpty();
    }

    @Override
    public String toString() {
        StringBuilder str = new StringBuilder();
        for (K key : dict.keySet()) {
            str.append(key.toString()).append(" -> ").append(dict.get(key).toString()).append("\n");
        }
        return str.toString();
    }

    @Override
    public Map<K, V> getContent() {
        return dict;
    }

    @Override
    public MyIDictionary<K, V> deepCopy() {
        MyDictionary<K, V> newDict = new MyDictionary<>();
        for (K key : dict.keySet()) {
            newDict.update(key, dict.get(key));
        }
        return newDict;
    }
}
