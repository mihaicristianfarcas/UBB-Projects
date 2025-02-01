package Model.Utils;

import Model.Exceptions.EmptyCollectionException;
import Model.Exceptions.InvalidKeyException;
import java.util.List;

public class MyList<T> implements MyIList<T> {
    private java.util.List<T> list;

    public MyList() {
        list = new java.util.ArrayList<>();
    }

    @Override
    public void add(T v) {
        list.add(v);
    }

    @Override
    public void remove(T v) throws EmptyCollectionException, InvalidKeyException {
        if (list.isEmpty()) {
            throw new EmptyCollectionException();
        }
        if (!list.contains(v)) {
            throw new InvalidKeyException("Key does not exist.");
        }
        list.remove(v);
    }

    @Override
    public boolean contains(T v) {
        return list.contains(v);
    }

    @Override
    public int size() {
        return list.size();
    }

    @Override
    public Iterable<T> getAll() {
        return list;
    }

    

    @Override
    public void clear() {
        list.clear();
    }

    @Override
    public String toString() {
        return list.toString();
    }

    @Override
    public List<T> getList() {
        return list;
    }
}
