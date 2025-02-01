package Model.Statements;

import Model.Values.Value;
import Model.PrgState;
import Model.Utils.MyIDictionary;
import Model.Types.IntType;
import Model.Types.Type;
import Model.Exceptions.MyInvalidTypeException;
import Model.Exceptions.UndefinedVariableException;
import Model.Values.IntValue;
import Model.Utils.MyILock;

public class NewLockStmt implements IStmt {
    private String var;

    public NewLockStmt(String var) {
        this.var = var;
    }

    @Override
    public PrgState execute(PrgState state) {
        MyIDictionary<String, Value> symTable = state.getSymTable();
        MyILock lockTable = state.getLockTable();

        if (!symTable.isDefined(var)) {
            throw new UndefinedVariableException(var);
        }

        if (!(symTable.lookup(var).getType() instanceof IntType)) {
            throw new MyInvalidTypeException("Lock: Variable must be of type IntType");
        }

        int newFreeLocation = lockTable.getFreeLocation();
        lockTable.update(newFreeLocation, -1);
        symTable.update(var, new IntValue(newFreeLocation));

        return null;
    }

    @Override
    public String toString() {
        return "newLock(" + var + ")";
    }

    @Override
    public IStmt deepCopy() {
        return null;
    }

    @Override
    public MyIDictionary<String, Type> typeCheck(MyIDictionary<String, Type> typeEnv) {
        if (!typeEnv.isDefined(var)) {
            throw new UndefinedVariableException(var);
        }

        if (!(typeEnv.lookup(var) instanceof IntType)) {
            throw new MyInvalidTypeException("Lock: Variable must be of type IntType");
        }

        return typeEnv;
    }

}
