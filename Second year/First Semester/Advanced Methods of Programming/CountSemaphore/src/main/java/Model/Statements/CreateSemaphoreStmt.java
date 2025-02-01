package Model.Statements;

import Model.Expressions.Exp;
import Model.Values.Value;
import Model.PrgState;
import Model.Utils.MyIDictionary;
import Model.Utils.MyIHeap;
import Model.Types.IntType;
import Model.Types.Type;
import Model.Exceptions.MyInvalidTypeException;
import Model.Exceptions.UndefinedVariableException;
import Model.Values.IntValue;
import Model.Utils.MySemaphore;
import java.util.ArrayList;
import Model.Utils.MyISemaphoreTable;

public class CreateSemaphoreStmt implements IStmt {
    private final String var;
    private final Exp expression;

    public CreateSemaphoreStmt(String var, Exp expression) {
        this.var = var;
        this.expression = expression;
    }

    @Override
    public PrgState execute(PrgState state) {
        MyIDictionary<String, Value> symTable = state.getSymTable();
        MyIHeap<Value> heap = state.getHeap();
        MyISemaphoreTable semaphoreTable = state.getSemaphoreTable();

        Value number = expression.eval(symTable, heap);

        if (!(number.getType().equals(new IntType()))) {
            throw new MyInvalidTypeException("Expression must evaluate to IntType");
        }

        if (!symTable.isDefined(var)) {
            throw new UndefinedVariableException(var.toString());
        }

        if (!symTable.lookup(var).getType().equals(new IntType())) {
            throw new MyInvalidTypeException("Variable must be of type IntType");
        }

        int permits = ((IntValue) number).getVal();
        int freeLocation = semaphoreTable.getFreeAddress();

        semaphoreTable.put(freeLocation, new MySemaphore(permits, new ArrayList<>()));
        symTable.update(var, new IntValue(freeLocation));

        return null;
    }

    @Override
    public MyIDictionary<String, Type> typeCheck(MyIDictionary<String, Type> typeEnv) throws MyInvalidTypeException {
        if (!typeEnv.lookup(var).equals(new IntType())) {
            throw new MyInvalidTypeException("Variable must be of type IntType");
        }

        Type expType = expression.typeCheck(typeEnv);
        if (!expType.equals(new IntType())) {
            throw new MyInvalidTypeException("Expression must evaluate to IntType");
        }

        return typeEnv;
    }

    @Override
    public IStmt deepCopy() {
        return new CreateSemaphoreStmt(var, expression.deepCopy());
    }

    @Override
    public String toString() {
        return "createSemaphore(" + var + ", " + expression.toString() + ")";
    }
}
