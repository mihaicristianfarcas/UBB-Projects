package Repository;

import Model.PrgState;
import Model.Types.Type;
import Model.Utils.MyDictionary;
import Model.Utils.MyIDictionary;

import java.util.ArrayList;
import java.util.List;

public class MyRepository implements MyIRepository {
    private List<PrgState> repository;

    public MyRepository() {
        this.repository = new ArrayList<>();
    }

    @Override
    public List<PrgState> getPrgList() {
        return repository;
    }

    @Override
    public void addProgram(PrgState program) {
        repository.add(program);
    }

    @Override
    public void setPrgList(List<PrgState> newRepository) {
        repository = newRepository;
    }

    @Override
    public void logPrgStateExec(PrgState prgState) {
        System.out.println(prgState.toString());
    }

    @Override
    public void runTypeChecker() {
        for (PrgState prg : repository) {
            MyIDictionary<String, Type> typeEnv = new MyDictionary<>();
            prg.getOriginalProgram().typeCheck(typeEnv);
        }
    }
}
