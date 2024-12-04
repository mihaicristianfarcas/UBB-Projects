package Model.Statements;

import Model.PrgState;

import java.io.IOException;

public interface IStmt {
    PrgState execute(PrgState state) throws RuntimeException, IOException;
    String toString();
    IStmt deepCopy();
}
