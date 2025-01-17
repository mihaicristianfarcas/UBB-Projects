package Model.Exceptions;

public class EmptyCollectionException extends RuntimeException {
    public EmptyCollectionException() {
        super("Error: Collection is empty.");
    }
}
