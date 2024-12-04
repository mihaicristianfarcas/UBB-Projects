package Model.Statements;

import Model.Exceptions.FileNotOpenException;
import Model.Exceptions.FileException;
import Model.Exceptions.MyInvalidTypeException;
import Model.Expressions.Exp;
import Model.PrgState;
import Model.Types.StringType;
import Model.Utils.MyIDictionary;
import Model.Utils.MyIHeap;
import Model.Values.StringValue;
import Model.Values.Value;

import java.io.BufferedReader;
import java.io.IOException;

public class CloseRFileStmt implements IStmt {
    private Exp exp;

    public CloseRFileStmt(Exp exp) {
        this.exp = exp;
    }

    @Override
    public PrgState execute(PrgState state) throws FileException, MyInvalidTypeException, IOException {
        MyIDictionary<String, Value> symTbl = state.getSymTable();
        MyIDictionary<StringValue, BufferedReader> fileTbl = state.getFileTable();
        MyIHeap<Value> heap = state.getHeap();

        Value val = exp.eval(symTbl, heap);
        if (!(val.getType() instanceof StringType)) {
            throw new MyInvalidTypeException("closeRFile error: Expression does not evaluate to StringType");
        }

        StringValue fileName = (StringValue) val;
        BufferedReader reader = fileTbl.lookup(fileName);
        if (reader == null) {
            throw new FileNotOpenException("closeRFile error: File " + fileName + " is not opened");
        }

        try {
            reader.close();
            fileTbl.remove(fileName);
        } catch (IOException exception) {
            throw new FileException(exception.toString());
        }

        return state;
    }

    @Override
    public String toString() {
        return "closeRFile(" + exp + ")";
    }

    @Override
    public IStmt deepCopy() {
        return null;
    }
}
