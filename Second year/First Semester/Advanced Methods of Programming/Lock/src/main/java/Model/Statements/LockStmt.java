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

public class LockStmt implements IStmt {
    private String var;

    public LockStmt(String var) {
        this.var = var;
    }

    @Override
    public PrgState execute(PrgState state) {
        MyIDictionary<String, Value> symTable = state.getSymTable();
        MyILock lockTable = state.getLockTable();

        if (!symTable.isDefined(var)) {
            throw new UndefinedVariableException(var.toString());
        }

        Value value = symTable.lookup(var);
        if (!value.getType().equals(new IntType())) {
            throw new MyInvalidTypeException("Variable " + var + " is not of type int");
        }

        int foundIndex = ((IntValue) value).getVal();

        if (!lockTable.containsKey(foundIndex)) {
            throw new UndefinedVariableException("Lock index " + foundIndex + " is not valid");
        } else if (lockTable.lookup(foundIndex) == -1) {
            lockTable.update(foundIndex, state.getCurrentID());
        } else {
            state.getExeStack().push(this);
        }

        return null;
    }

    @Override
    public String toString() {
        return "lock(" + var + ")";
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
