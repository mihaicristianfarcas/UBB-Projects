package Model.Statements;

import Model.PrgState;

import java.io.IOException;

public class ForkStmt implements IStmt {

    @Override
    public PrgState execute(PrgState state) throws RuntimeException, IOException {
        return null;
    }

    @Override
    public IStmt deepCopy() {
        return null;
    }
}
