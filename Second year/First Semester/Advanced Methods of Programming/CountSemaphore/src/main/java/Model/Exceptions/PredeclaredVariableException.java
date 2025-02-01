package Model.Exceptions;

public class PredeclaredVariableException extends RuntimeException {
    public PredeclaredVariableException(String name) {
        super("Variable " + name + " already declared");
    }
}
