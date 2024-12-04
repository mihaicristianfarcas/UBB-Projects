package Model.Utils;

import Model.Exceptions.EmptyCollectionException;

public class MyStack<T> implements MyIStack<T> {
    private java.util.Stack<T> stack;

    public MyStack() {
        stack = new java.util.Stack<>();
    }

    @Override
    public void push(T v) {
        stack.push(v);
    }

    @Override
    public T pop() throws EmptyCollectionException {
        if (stack.isEmpty()) {
            throw new EmptyCollectionException();
        }
        return stack.pop();
    }

    @Override
    public boolean isEmpty() {
        return stack.isEmpty();
    }

    @Override
    public T top() throws EmptyCollectionException {
        if (stack.isEmpty()) {
            throw new EmptyCollectionException();
        }
        return stack.peek();
    }

    @Override
    public int size() {
        return stack.size();
    }

    @Override
    public void clear() {
        stack.clear();
    }

    @Override
    public Iterable<T> getAll() {
        return stack;
    }

    @Override
    public String toString() {
        return stack.toString() + "\n";
    }
}
