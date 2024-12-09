package Model.Expressions;

import Model.Utils.MyIDictionary;
import Model.Utils.MyIHeap;
import Model.Values.Value;

public interface Exp {
    Value eval(MyIDictionary<String, Value> tbl, MyIHeap<Value> heap) throws RuntimeException;
    Exp deepCopy();
    // TODO Type typecheck(MyIDictionary<String,Type> typeEnv) throws MyException;
}
