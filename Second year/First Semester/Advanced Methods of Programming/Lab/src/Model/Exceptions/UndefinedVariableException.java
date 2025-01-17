package Model.Exceptions;

public class UndefinedVariableException extends RuntimeException {
    public UndefinedVariableException(String var) {
        super("Error: Undefined variable: " + var);
    }
}
