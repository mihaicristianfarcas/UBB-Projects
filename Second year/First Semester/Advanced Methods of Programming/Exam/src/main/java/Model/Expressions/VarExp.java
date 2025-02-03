package Model.Expressions;

import Model.Exceptions.MyInvalidTypeException;
import Model.Exceptions.UndefinedVariableException;
import Model.Types.Type;
import Model.Utils.MyIDictionary;
import Model.Utils.MyIHeap;
import Model.Values.Value;

public class VarExp implements Exp {
    private String id;

    public VarExp(String id) {
        this.id = id;
    }

    @Override
    public Value eval(MyIDictionary<String, Value> tbl, MyIHeap<Value> heap) throws UndefinedVariableException {
        if(tbl.isDefined(id))
            return tbl.lookup(id);
        else
            throw new UndefinedVariableException(id);
    }

    @Override
    public Exp deepCopy() {
        return new VarExp(id);
    }

    @Override
    public Type typeCheck(MyIDictionary<String, Type> typeEnv) throws MyInvalidTypeException {
        return typeEnv.lookup(id);
    }

    @Override
    public String toString() {
        return id;
    }
}
