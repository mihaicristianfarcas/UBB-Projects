package Repository;

import Model.PrgState;

import java.io.IOException;
import java.util.List;

public interface MyIRepository {
    List<PrgState> getPrgList();
    void addProgram(PrgState program);
    void setPrgList(List<PrgState> newRepository);
    void logPrgStateExec(PrgState prgState) throws IOException;
    void runTypeChecker() throws Exception;
}
