package Model.Statements;

import Model.Exceptions.MyInvalidTypeException;
import Model.PrgState;
import Model.Types.Type;
import Model.Utils.MyIDictionary;

import java.io.IOException;

public interface IStmt {
    PrgState execute(PrgState state) throws RuntimeException, IOException;
    String toString();
    IStmt deepCopy();
    MyIDictionary<String, Type> typeCheck(MyIDictionary<String, Type> typeEnv) throws MyInvalidTypeException;
}
