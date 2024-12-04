package Model.Statements;

import Model.Exceptions.FileAlreadyOpenException;
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
import java.io.FileReader;
import java.io.IOException;

public class OpenRFileStmt implements IStmt {
    private Exp exp;

    public OpenRFileStmt(Exp exp) {
        this.exp = exp;
    }

    @Override
    public PrgState execute(PrgState state) throws FileException, MyInvalidTypeException, IOException {
        MyIDictionary<String, Value> symTbl = state.getSymTable();
        MyIDictionary<StringValue, BufferedReader> fileTbl = state.getFileTable();
        MyIHeap<Value> heap = state.getHeap();

        Value val = exp.eval(symTbl, heap);
        if (!(val.getType() instanceof StringType)) {
            throw new MyInvalidTypeException("openRFile error: Expression does not evaluate to StringType");
        }

        StringValue fileName = (StringValue) val;
        if (state.getFileTable().isDefined(fileName)) {
            throw new FileAlreadyOpenException("openRFile error: File " + fileName + " is already opened");
        }

        try {
            BufferedReader reader = new BufferedReader(new FileReader(fileName.getVal()));
            fileTbl.update(fileName, reader);
        } catch (IOException exception) {
            throw new FileException(exception.toString());
        }

        return state;
    }

    @Override
    public String toString() {
        return "openRFile(" + exp + ")";
    }

    @Override
    public IStmt deepCopy() {
        return null;
    }
}
