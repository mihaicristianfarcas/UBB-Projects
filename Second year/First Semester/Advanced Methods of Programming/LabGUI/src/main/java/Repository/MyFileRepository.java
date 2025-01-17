package Repository;

import Model.PrgState;
import Model.Types.Type;
import Model.Utils.MyDictionary;
import Model.Utils.MyIDictionary;

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

    @Override
    public void logPrgStateExec(PrgState prgState) throws IOException {
        PrintWriter logFile = new PrintWriter(new BufferedWriter(new FileWriter(logFilePath, true)));
        logFile.write(prgState.toString());
        logFile.close();
    }

    @Override
    public void runTypeChecker() throws Exception {
        MyIDictionary<String, Type> typeEnv = new MyDictionary<>();
        repository.get(0).getOriginalProgram().typeCheck(typeEnv);
    }

    @Override
    public void clear() {
        repository.clear();
    }
}
