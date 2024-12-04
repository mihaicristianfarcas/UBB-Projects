package Model.Statements;
import Model.Exceptions.MyInvalidTypeException;
import Model.Expressions.Exp;
import Model.PrgState;
import Model.Types.BoolType;
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

        return state;
    }

    @Override
    public String toString() {
        return "while(" + exp.toString() + ") {" + stmt.toString() + "}";
    }

    @Override
    public IStmt deepCopy() {
        return new WhileStmt( exp.deepCopy(), stmt.deepCopy());
    }
}
