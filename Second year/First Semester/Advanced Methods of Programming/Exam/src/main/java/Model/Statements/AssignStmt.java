package Model.Statements;

import Model.Exceptions.MyInvalidTypeException;
import Model.Exceptions.UndefinedVariableException;
import Model.PrgState;
import Model.Types.Type;
import Model.Utils.MyIDictionary;
import Model.Utils.MyIHeap;
import Model.Values.Value;
import Model.Expressions.Exp;

public class AssignStmt implements IStmt {
    private String id;
    private Exp exp;

    public AssignStmt(String id, Exp exp) {
        this.id = id;
        this.exp = exp;
    }

    @Override
    public PrgState execute(PrgState state) throws MyInvalidTypeException, UndefinedVariableException {
        MyIDictionary<String, Value> symTbl = state.getSymTable();
        MyIHeap<Value> heap = state.getHeap();

        if (symTbl.isDefined(id)) {
            Value val = exp.eval(symTbl, heap);
            Type valType = val.getType();
            Type type = (symTbl.lookup(id)).getType();
            if (valType.equals(type))
                symTbl.update(id, val);
            else
                throw new MyInvalidTypeException("Declared type of variable '" + id + "' and type of the assigned expression do not match");
        }
        else throw new UndefinedVariableException(id);

        return null;
    }

    @Override
    public String toString () {
        return id + "=" + exp.toString();
    }

    @Override
    public IStmt deepCopy() {
        return new AssignStmt(id, exp.deepCopy());
    }

    @Override
    public MyIDictionary<String, Type> typeCheck(MyIDictionary<String, Type> typeEnv) throws MyInvalidTypeException {
        Type varType = typeEnv.lookup(id);
        Type expType = exp.typeCheck(typeEnv);
        if (varType.equals(expType))
            return typeEnv;
        else
            throw new MyInvalidTypeException("Assign: Declared type of variable '" + id + "' and type of the assigned expression do not match");
    }
}

