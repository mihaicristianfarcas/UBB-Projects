package Model.Expressions;

import Model.Exceptions.MyInvalidTypeException;
import Model.Types.Type;
import Model.Utils.MyIDictionary;
import Model.Utils.MyIHeap;
import Model.Values.Value;

public interface Exp {
    Value eval(MyIDictionary<String, Value> tbl, MyIHeap<Value> heap) throws RuntimeException;
    Exp deepCopy();
    Type typeCheck(MyIDictionary<String, Type> typeEnv) throws MyInvalidTypeException;
}
