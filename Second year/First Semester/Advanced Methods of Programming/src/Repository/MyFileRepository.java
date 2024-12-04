package Repository;

import Model.PrgState;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

public class MyFileRepository implements MyIRepository {
    private List<PrgState> repository;
    private final String logFilePath;

    public MyFileRepository(String logFilePath) {
        this.repository = new ArrayList<>();
        this.logFilePath = logFilePath;
    }

    @Override
    public List<PrgState> getPrgList() {
        return repository;
    }

    @Override
    public void setPrgList(List<PrgState> newRepository) {
        repository = newRepository;
    }

    @Override
    public void addProgram(PrgState program) {
        repository.add(program);
    }

    // TODO check this method
    @Override
    public void logPrgStateExec(PrgState prgState) throws IOException {
        PrintWriter logFile = new PrintWriter(new BufferedWriter(new FileWriter(logFilePath, true)));
        logFile.write(prgState.toString());
        logFile.close();
    }
}
