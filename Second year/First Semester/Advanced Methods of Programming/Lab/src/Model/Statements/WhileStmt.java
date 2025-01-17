package Model.Statements;
import Model.Exceptions.MyInvalidTypeException;
import Model.Expressions.Exp;
import Model.PrgState;
import Model.Types.BoolType;
import Model.Types.Type;
import Model.Utils.MyIDictionary;
import Model.Utils.MyIHeap;
import Model.Values.Value;

public class WhileStmt implements IStmt {
    private IStmt stmt;
    private Exp exp;

    public WhileStmt(Exp exp, IStmt stmt) {
        this.stmt = stmt;
        this.exp = exp;
    }

    @Override
    public PrgState execute(PrgState state) throws MyInvalidTypeException {
        MyIDictionary<String, Value> symTbl = state.getSymTable();
        MyIHeap<Value> heap = state.getHeap();

        Value val = exp.eval(symTbl, heap);
        if (val.getType().equals(new BoolType())) {
            if (val.toString().equals("true")) {
                state.getExeStack().push(this);
                state.getExeStack().push(stmt);
            }
        }
        else {
            throw new MyInvalidTypeException("condition expression is not a boolean");
        }

        return null;
    }

    @Override
    public String toString() {
        return "while(" + exp.toString() + ") {" + stmt.toString() + "}";
    }

    @Override
    public IStmt deepCopy() {
        return new WhileStmt( exp.deepCopy(), stmt.deepCopy());
    }

    @Override
    public MyIDictionary<String, Type> typeCheck(MyIDictionary<String, Type> typeEnv) throws MyInvalidTypeException {
        Type expType = exp.typeCheck(typeEnv);
        if (expType.equals(new BoolType())) {
            stmt.typeCheck(typeEnv.deepCopy());
            return typeEnv;
        }
        else {
            throw new MyInvalidTypeException("While: condition expression is not a boolean");
        }
    }
}
