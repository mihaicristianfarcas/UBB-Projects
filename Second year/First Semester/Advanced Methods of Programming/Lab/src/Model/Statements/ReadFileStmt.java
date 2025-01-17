package Model.Statements;

import Model.Exceptions.FileNotOpenException;
import Model.Exceptions.FileException;
import Model.Exceptions.MyInvalidTypeException;
import Model.Expressions.Exp;
import Model.PrgState;
import Model.Types.IntType;
import Model.Types.StringType;
import Model.Types.Type;
import Model.Utils.MyIDictionary;
import Model.Utils.MyIHeap;
import Model.Values.IntValue;
import Model.Values.StringValue;
import Model.Values.Value;
import java.io.BufferedReader;
import java.io.IOException;

public class ReadFileStmt implements IStmt {
    private Exp exp;
    private String id;

    public ReadFileStmt(Exp exp, String id) {
        this.exp = exp;
        this.id = id;
    }

    @Override
    public PrgState execute(PrgState state) throws FileException, MyInvalidTypeException, IOException {
        MyIDictionary<String, Value> symTbl = state.getSymTable();
        MyIDictionary<StringValue, BufferedReader> fileTbl = state.getFileTable();
        MyIHeap<Value> heap = state.getHeap();

        if (!symTbl.isDefined(id) || !(symTbl.lookup(id).getType() instanceof IntType)) {
            throw new MyInvalidTypeException("readFile error: Variable " + id + " is not defined as int in SymTable");
        }

        Value val = exp.eval(symTbl, heap);
        if (!(val.getType() instanceof StringType)) {
            throw new MyInvalidTypeException("readFile error: Expression does not evaluate to StringType");
        }

        StringValue fileName = (StringValue) val;
        BufferedReader reader = fileTbl.lookup(fileName);
        if (reader == null) {
            throw new FileNotOpenException("ReadFile error: File " + fileName + " is not opened");
        }

        try {
            String line = reader.readLine();
            int intValue;
            if (line == null) {
                intValue = 0;
            } else {
                intValue = Integer.parseInt(line);
            }

            symTbl.update(id, new IntValue(intValue));
        } catch (IOException exception) {
            throw new FileException(exception.toString());
        }

        return null;
    }

    @Override
    public IStmt deepCopy() {
        return null;
    }

    @Override
    public MyIDictionary<String, Type> typeCheck(MyIDictionary<String, Type> typeEnv) throws MyInvalidTypeException {
        Type expType = exp.typeCheck(typeEnv);
        if (!(expType instanceof StringType)) {
            throw new MyInvalidTypeException("readFile error: Expression does not evaluate to StringType");
        }

        if (!typeEnv.isDefined(id)) {
            throw new MyInvalidTypeException("readFile error: Variable " + id + " is not defined in SymTable");
        }

        if (!(typeEnv.lookup(id) instanceof IntType)) {
            throw new MyInvalidTypeException("readFile error: Variable " + id + " is not defined as int in SymTable");
        }

        return typeEnv;
    }

    @Override
    public String toString() {
        return "readFile(" + exp + ")";
    }
}
