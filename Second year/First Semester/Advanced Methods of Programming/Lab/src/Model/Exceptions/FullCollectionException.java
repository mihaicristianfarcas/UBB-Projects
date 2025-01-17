package Model.Exceptions;

public class FullCollectionException extends RuntimeException {
    public FullCollectionException() {
        super("Error: Collection is full.");
    }
}
