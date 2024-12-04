package Model.Utils;

public interface MyIList<T> {
    void add(T v);
    void remove(T v);
    boolean contains(T v);
    int size();
    Iterable<T> getAll();
    void clear();
}
