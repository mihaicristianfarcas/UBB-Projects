package Model.Exceptions;

public class DivisionByZeroException extends RuntimeException {
    public DivisionByZeroException() {
        super("Error: Division by zero.");
    }
}
