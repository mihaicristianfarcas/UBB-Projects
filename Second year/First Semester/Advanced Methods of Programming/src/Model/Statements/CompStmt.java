package Model.Statements;

import Model.Exceptions.MyInvalidTypeException;
import Model.PrgState;
import Model.Types.Type;
import Model.Utils.MyIDictionary;
import Model.Utils.MyIStack;

public class CompStmt implements IStmt {
    private IStmt first;
    private IStmt second;

    public CompStmt(IStmt first, IStmt second) {
        this.first = first;
        this.second = second;
    }

    @Override
    public PrgState execute(PrgState state){
        MyIStack<IStmt> stack = state.getExeStack();
        stack.push(second);
        stack.push(first);
        return null;
    }

    @Override
    public String toString() {
        return "(" + first.toString() + ";" + second.toString() + ")";
    }

    @Override
    public IStmt deepCopy() {
        return new CompStmt(first.deepCopy(), second.deepCopy());
    }

    @Override
    public MyIDictionary<String, Type> typeCheck(MyIDictionary<String, Type> typeEnv) throws MyInvalidTypeException {
        return second.typeCheck(first.typeCheck(typeEnv));
    }
}
