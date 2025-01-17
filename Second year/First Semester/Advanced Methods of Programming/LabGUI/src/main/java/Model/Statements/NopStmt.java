package Model.Statements;

import Model.Exceptions.EmptyExecutionStackException;
import Model.Exceptions.MyInvalidTypeException;
import Model.PrgState;
import Model.Types.Type;
import Model.Utils.MyIDictionary;

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

    @Override
    public MyIDictionary<String, Type> typeCheck(MyIDictionary<String, Type> typeEnv) throws MyInvalidTypeException {
        return typeEnv;
    }
}
