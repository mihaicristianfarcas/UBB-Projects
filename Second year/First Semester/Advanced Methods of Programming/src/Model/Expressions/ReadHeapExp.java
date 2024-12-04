package Model.Expressions;

import Model.Exceptions.InvalidKeyException;
import Model.Exceptions.MyInvalidTypeException;
import Model.Utils.MyIDictionary;
import Model.Utils.MyIHeap;
import Model.Values.RefValue;
import Model.Values.Value;

public class ReadHeapExp implements Exp {
    private Exp exp;

    public ReadHeapExp(Exp v) {
        exp = v;
    }

    public Value eval(MyIDictionary<String,Value> tbl, MyIHeap<Value> heap) throws MyInvalidTypeException, InvalidKeyException {
        Value val = exp.eval(tbl, heap);
        if(val instanceof RefValue ref) {
            int address = ref.getAddress();
            if (heap.isDefined(address))
                return heap.lookup(address);
            else
                throw new InvalidKeyException("Address '" + address + "' is not defined in heap");
        }
        else
            throw new MyInvalidTypeException("Expression could not be evaluated to a RefValue");
    }

    @Override
    public Exp deepCopy() {
        return null;
    }

    @Override
    public String toString() {
        return "readHeap(" + exp + ")";
    }
}
