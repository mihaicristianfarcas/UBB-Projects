package Model.Statements;

import Model.Exceptions.MyInvalidTypeException;
import Model.Exceptions.UndefinedVariableException;
import Model.Expressions.Exp;
import Model.PrgState;
import Model.Types.RefType;
import Model.Types.Type;
import Model.Utils.MyIDictionary;
import Model.Utils.MyIHeap;
import Model.Values.RefValue;
import Model.Values.Value;

public class NewStmt implements IStmt {
    private String varName;
    private Exp exp;

    public NewStmt(String varName, Exp exp) {
        this.varName = varName;
        this.exp = exp;
    }

    // TODO check this method
    @Override
    public PrgState execute(PrgState state) throws MyInvalidTypeException, UndefinedVariableException {
        MyIDictionary<String, Value> symTbl = state.getSymTable();
        MyIHeap<Value> heap = state.getHeap();

        if(symTbl.isDefined(varName)){
            if(symTbl.lookup(varName).getType() instanceof RefType) {
                Value val = exp.eval(symTbl, heap);
                Type valType = val.getType();
                if(((RefType)symTbl.lookup(varName).getType()).getInner().equals(valType)) {
                    int address = heap.allocate(val);
                    symTbl.update(varName, new RefValue(address, valType));
                }
                else
                    throw new MyInvalidTypeException("Declared type of variable '" + varName + "' and type of the assigned expression do not match");
            }
            else
                throw new MyInvalidTypeException("Declared type of variable '" + varName + "' is not a RefType");
        }
        else throw new UndefinedVariableException(varName);
        return null;
    }

    @Override
    public String toString() {
        return "new(" + varName + ", " + exp + ")";
    }

    @Override
    public IStmt deepCopy() {
        return new NewStmt(varName, exp);
    }
}
