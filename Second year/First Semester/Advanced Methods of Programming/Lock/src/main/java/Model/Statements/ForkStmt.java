package Model.Statements;

import Model.Exceptions.MyInvalidTypeException;
import Model.PrgState;
import Model.Types.Type;
import Model.Utils.MyIDictionary;
import Model.Values.Value;

public class ForkStmt implements IStmt {
    private final IStmt forkBody;

    public ForkStmt(IStmt forkBody) {
        this.forkBody = forkBody;
    }

    @Override
    public PrgState execute(PrgState parentState) {
        PrgState newPrgState = new PrgState(forkBody);

        MyIDictionary<String, Value> clonedSymTable = parentState.getSymTable().deepCopy();
        newPrgState.setSymTable(clonedSymTable);
        newPrgState.setHeap(parentState.getHeap());
        newPrgState.setFileTable(parentState.getFileTable());
        newPrgState.setOut(parentState.getOut());
        newPrgState.setLockTable(parentState.getLockTable());

        return newPrgState;
    }

    @Override
    public String toString() {
        return "fork(" + forkBody.toString() + ")";
    }

    @Override
    public IStmt deepCopy() {
        return new ForkStmt(forkBody.deepCopy());
    }

    @Override
    public MyIDictionary<String, Type> typeCheck(MyIDictionary<String, Type> typeEnv) throws MyInvalidTypeException {
        forkBody.typeCheck(typeEnv.deepCopy());
        return typeEnv;
    }
}
