package Model.Statements;

import Model.Exceptions.InvalidKeyException;
import Model.Exceptions.MyInvalidTypeException;
import Model.Expressions.Exp;
import Model.PrgState;
import Model.Utils.MyIDictionary;
import Model.Utils.MyIHeap;
import Model.Values.RefValue;
import Model.Values.Value;

import java.io.IOException;

public class WriteHeapStmt implements IStmt {
    private String varName;
    private Exp exp;

    public WriteHeapStmt(String varName, Exp exp) {
        this.varName = varName;
        this.exp = exp;
    }

    @Override
    public PrgState execute(PrgState state) throws IOException {
        MyIDictionary<String, Value> symTbl = state.getSymTable();
        MyIHeap<Value> heap = state.getHeap();

        if (symTbl.isDefined(varName)) {
            Value val = symTbl.lookup(varName);
            if (val instanceof RefValue ref) {
                int address = ref.getAddress();
                if (heap.isDefined(address)) {
                    Value expVal = exp.eval(symTbl, heap);
                    Value heapVal = heap.lookup(address);
                    if (heapVal.getType().equals(expVal.getType())) {
                        heap.update(address, expVal);
                    } else {
                        throw new MyInvalidTypeException("The type of the value in the heap does not match the type of the value to be written");
                    }
                } else {
                    throw new InvalidKeyException("The address does not exist in the heap");
                }
            } else {
                throw new MyInvalidTypeException("Value could not be evaluated to a RefValue");
            }
        } else {
            throw new InvalidKeyException("The variable is not defined in the symbol table");
        }

        return null;
    }

    @Override
    public String toString() {
        return "writeHeap(" + varName + ", " + exp + ")";
    }

    @Override
    public IStmt deepCopy() {
        return new WriteHeapStmt(varName, exp.deepCopy());
    }
}
