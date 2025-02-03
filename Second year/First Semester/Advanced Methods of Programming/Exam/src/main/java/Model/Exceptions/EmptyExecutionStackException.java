package Model.Exceptions;

public class EmptyExecutionStackException extends RuntimeException {
    public EmptyExecutionStackException() {
        super("Error: Attempting to execute with an empty execution stack.");
    }
}
