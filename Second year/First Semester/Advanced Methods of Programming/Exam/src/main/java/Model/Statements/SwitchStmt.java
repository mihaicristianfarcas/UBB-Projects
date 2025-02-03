package Model.Statements;

import Model.Exceptions.InvalidExpressionException;
import Model.Exceptions.MyInvalidTypeException;
import Model.PrgState;
import Model.Types.Type;
import Model.Utils.MyIDictionary;
import Model.Expressions.Exp;
import Model.Expressions.RelExp;
import Model.Utils.MyIStack;

public class SwitchStmt implements IStmt {
    private Exp exp, exp1, exp2;
    private IStmt case1, case2, defaultCase;

    public SwitchStmt(Exp exp, Exp exp1, IStmt case1, Exp exp2, IStmt case2, IStmt defaultCase) {
        this.exp = exp;
        this.exp1 = exp1;
        this.case1 = case1;
        this.exp2 = exp2;
        this.case2 = case2;
        this.defaultCase = defaultCase;

    }

    @Override
    public PrgState execute(PrgState state) throws InvalidExpressionException {
        MyIStack<IStmt> stack = state.getExeStack();
        Exp relExp1 = new RelExp("==", exp, exp1);
        Exp relExp2 = new RelExp("==", exp, exp2);
        IStmt ifStmt = new IfStmt(relExp1, case1, (new IfStmt(relExp2, case2, defaultCase)));
        stack.push(ifStmt);
        return null;
    }

    @Override
    public String toString() {
        return "switch(" + exp.toString() + ") (case  " + exp1.toString() + ": " + case1.toString() + ") (case "
                + exp1.toString() + ": " + case2.toString() + ") (default: " + defaultCase.toString() + ")";
    }

    @Override
    public IStmt deepCopy() {
        return new SwitchStmt(exp.deepCopy(), exp1.deepCopy(), case1.deepCopy(), exp2.deepCopy(), case2.deepCopy(),
                defaultCase.deepCopy());
    }

    @Override
    public MyIDictionary<String, Type> typeCheck(MyIDictionary<String, Type> typeEnv) throws MyInvalidTypeException {
        Type expType = exp.typeCheck(typeEnv);
        Type expType1 = exp1.typeCheck(typeEnv);
        Type expType2 = exp2.typeCheck(typeEnv);

        if (expType.equals(expType1) && expType1.equals(expType2)) {
            case1.typeCheck(typeEnv.deepCopy());
            case2.typeCheck(typeEnv.deepCopy());
            defaultCase.typeCheck(typeEnv.deepCopy());
            return typeEnv;
        } else {
            throw new MyInvalidTypeException("Switch: expressions do not have the same type");
        }
    }
}
