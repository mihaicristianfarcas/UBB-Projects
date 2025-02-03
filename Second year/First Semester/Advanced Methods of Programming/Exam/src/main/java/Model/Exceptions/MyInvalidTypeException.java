package Model.Exceptions;

public class MyInvalidTypeException extends RuntimeException {
    public MyInvalidTypeException(String message) {
        super(message);
    }
}
