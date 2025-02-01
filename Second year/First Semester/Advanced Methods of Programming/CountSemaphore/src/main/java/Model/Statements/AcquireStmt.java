package Model.Statements;

import Model.PrgState;
import Model.Exceptions.MyInvalidTypeException;
import Model.Exceptions.UndefinedVariableException;
import Model.Utils.MyIDictionary;
import Model.Utils.MyISemaphoreTable;
import Model.Values.Value;
import Model.Types.Type;
import Model.Types.IntType;
import Model.Values.IntValue;
import Model.Utils.MySemaphore;
import java.util.List;

public class AcquireStmt implements IStmt {
    private final String var;

    public AcquireStmt(String var) {
        this.var = var;
    }

    @Override
    public PrgState execute(PrgState state) {
        MyIDictionary<String, Value> symTable = state.getSymTable();
        MyISemaphoreTable semaphoreTable = state.getSemaphoreTable();

        if (!symTable.isDefined(var)) {
            throw new UndefinedVariableException(var.toString());
        }

        Value value = symTable.lookup(var);
        if (!value.getType().equals(new IntType())) {
            throw new MyInvalidTypeException("Variable " + var + " is not of type int");
        }

        int foundIndex = ((IntValue) value).getVal();

        if (!semaphoreTable.containsKey(foundIndex)) {
            throw new UndefinedVariableException("Semaphore index " + foundIndex + " is not valid");
        }

        MySemaphore semaphore = semaphoreTable.get(foundIndex);
        int permits = semaphore.getPermits();
        List<Integer> acquiredList = semaphore.getAcquired();

        if (acquiredList.contains(state.getCurrentID())) {
            return null;
        }

        if (permits > acquiredList.size()) {
            acquiredList.add(state.getCurrentID());
            return null;
        }

        state.getExeStack().push(this);
        return null;
    }

    @Override
    public String toString() {
        return "acquire(" + var + ")";
    }

    @Override
    public IStmt deepCopy() {
        return new AcquireStmt(var);
    }

    @Override
    public MyIDictionary<String, Type> typeCheck(MyIDictionary<String, Type> typeEnv) throws MyInvalidTypeException {
        Type varType = typeEnv.lookup(var);
        if (!varType.equals(new IntType())) {
            throw new MyInvalidTypeException("Variable " + var + " is not of type int");
        }
        return typeEnv;
    }
}