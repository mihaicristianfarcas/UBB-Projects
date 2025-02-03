package Model.Expressions;

import Model.Exceptions.DivisionByZeroException;
import Model.Exceptions.MyInvalidTypeException;
import Model.Types.IntType;
import Model.Types.Type;
import Model.Utils.MyIDictionary;
import Model.Utils.MyIHeap;
import Model.Values.IntValue;
import Model.Values.Value;


public class ArithExp implements Exp {
    private Exp e1;
    private Exp e2;
    private char op;

    public ArithExp(char op, Exp e1, Exp e2) {
        this.e1 = e1;
        this.e2 = e2;
        this.op = op;
    }

    @Override
    public Value eval(MyIDictionary<String, Value> tbl, MyIHeap<Value> heap) throws MyInvalidTypeException, DivisionByZeroException {
        Value v1, v2;
        v1 = e1.eval(tbl, heap);
        if (v1.getType().equals(new IntType())) {
            v2 = e2.eval(tbl, heap);
            if (v2.getType().equals(new IntType())) {
                IntValue i1 = (IntValue) v1;
                IntValue i2 = (IntValue) v2;
                int n1, n2;
                n1 = i1.getVal();
                n2 = i2.getVal();
                if (op == '+') return new IntValue(n1 + n2);
                if (op == '-') return new IntValue(n1 - n2);
                if (op == '*') return new IntValue(n1 * n2);
                if (n2 == 0) throw new DivisionByZeroException();
                return new IntValue(n1 / n2);
            } else
                throw new MyInvalidTypeException("second operand is not an integer");
        } else
            throw new MyInvalidTypeException("first operand is not an integer");
    }

    @Override
    public Exp deepCopy() {
        return new ArithExp(op, e1.deepCopy(), e2.deepCopy());
    }

    @Override
    public Type typeCheck(MyIDictionary<String, Type> typeEnv) throws MyInvalidTypeException {
        Type t1, t2;
        t1 = e1.typeCheck(typeEnv);
        t2 = e2.typeCheck(typeEnv);
        if (t1.equals(new IntType())) {
            if (t2.equals(new IntType())) {
                return new IntType();
            } else
                throw new MyInvalidTypeException("ArithExp: second operand is not an integer");
        } else
            throw new MyInvalidTypeException("ArithExp: first operand is not an integer");
    }

    @Override
    public String toString() {
        String s = "";
        if (op == '+') s = "+";
        if (op == '-') s = "-";
        if (op == '*') s = "*";
        if (op == '/') s = "/";
        return e1.toString() + s + e2.toString();
    }
}
