package Model.Statements;

import Model.Exceptions.EmptyExecutionStackException;
import Model.PrgState;

public class NopStmt implements IStmt {
    @Override
    public PrgState execute(PrgState state) throws EmptyExecutionStackException {
        if(state.getExeStack().isEmpty())
            throw new EmptyExecutionStackException();
        return null;
    }

    @Override
    public String toString() {
        return "nop";
    }

    @Override
    public IStmt deepCopy() {
        return new NopStmt();
    }
}
