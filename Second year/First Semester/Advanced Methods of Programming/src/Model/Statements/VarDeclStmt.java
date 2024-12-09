package Model.Statements;
import Model.Exceptions.PredeclaredVariableException;
import Model.PrgState;
import Model.Types.IntType;
import Model.Types.Type;
import Model.Utils.MyIDictionary;
import Model.Values.Value;

public class VarDeclStmt implements IStmt {
    private String name;
    private Type type;

    public VarDeclStmt(String name, Type type) {
        this.name = name;
        this.type = type;
    }

    @Override
    public PrgState execute(PrgState state) throws PredeclaredVariableException {
        MyIDictionary<String, Value> symTbl = state.getSymTable();
        if (symTbl.isDefined(name)) {
            throw new PredeclaredVariableException(name);
        }
        symTbl.update(name, type.defaultValue());
        return null;
    }

    @Override
    public String toString() {
        return type.toString() + " " + name;
    }

    @Override
    public IStmt deepCopy() {
        return new VarDeclStmt(name, new IntType());
    }
}
