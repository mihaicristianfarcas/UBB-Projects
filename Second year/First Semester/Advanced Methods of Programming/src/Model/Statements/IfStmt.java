package Model.Statements;
import Model.Exceptions.InvalidExpressionException;
import Model.Expressions.Exp;
import Model.PrgState;
import Model.Utils.MyIDictionary;
import Model.Utils.MyIHeap;
import Model.Utils.MyIStack;
import Model.Values.Value;

public class IfStmt implements IStmt {
    private Exp exp;
    private IStmt thenS;
    private IStmt elseS;

    public IfStmt(Exp exp, IStmt thenS, IStmt elseS) {
        this.exp = exp;
        this.thenS = thenS;
        this.elseS = elseS;
    }

    @Override
    public PrgState execute(PrgState state) throws InvalidExpressionException {
        MyIStack<IStmt> stack = state.getExeStack();
        MyIDictionary<String, Value> symTbl = state.getSymTable();
        MyIHeap<Value> heap = state.getHeap();

        Value val = exp.eval(symTbl, heap);
        if (val.getType().toString().equals("bool")) {
            if (val.toString().equals("true")) {
                stack.push(thenS);
            }
            else {
                stack.push(elseS);
            }
        }
        else {
            throw new InvalidExpressionException("condition expression is not a boolean");
        }
        return null;
    }

    @Override
    public String toString() {
        return "if(" + exp.toString() + ") then(" + thenS.toString() + ") else(" + elseS.toString() + ")";
    }

    @Override
    public IStmt deepCopy() {
        return new IfStmt(exp.deepCopy(), thenS.deepCopy(), elseS.deepCopy());
    }
}
