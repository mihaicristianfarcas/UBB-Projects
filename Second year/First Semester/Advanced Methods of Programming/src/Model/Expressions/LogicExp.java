package Model.Expressions;

import Model.Exceptions.MyInvalidTypeException;
import Model.Types.BoolType;
import Model.Utils.MyIDictionary;
import Model.Utils.MyIHeap;
import Model.Values.BoolValue;
import Model.Values.Value;

public class LogicExp implements Exp {
    private Exp e1;
    private Exp e2;
    private int op; // 1 - &&, 2 - ||

    public LogicExp(Exp e1, Exp e2, int op) {
        this.e1 = e1;
        this.e2 = e2;
        this.op = op;
    }

    @Override
    public Value eval(MyIDictionary<String, Value> tbl, MyIHeap<Value> heap) throws MyInvalidTypeException {
        Value v1, v2;
        v1 = e1.eval(tbl, heap);
        if (v1.getType().equals(new BoolType())) {
            v2 = e2.eval(tbl, heap);
            if (v2.getType().equals(new BoolType())) {
                BoolValue i1 = (BoolValue) v1;
                BoolValue i2 = (BoolValue) v2;
                boolean n1, n2;
                n1 = i1.getVal();
                n2 = i2.getVal();
                if (op == 1) return new BoolValue(n1 && n2);
                if (op == 2) return new BoolValue(n1 || n2);
            } else throw new MyInvalidTypeException("second operand is not a boolean");
        } else throw new MyInvalidTypeException("first operand is not a boolean");
        return new BoolValue(false);
    }

    @Override
    public Exp deepCopy() {
        return new LogicExp(e1.deepCopy(), e2.deepCopy(), op);
    }

    @Override
    public String toString() {
        String s = "";
        if (op == 1) s += e1.toString() + " && " + e2.toString();
        if (op == 2) s += e1.toString() + " || " + e2.toString();
        return s;
    }
}
