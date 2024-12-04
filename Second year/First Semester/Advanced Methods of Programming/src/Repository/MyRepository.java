package Repository;

import Model.PrgState;
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

    // TODO check this method
    @Override
    public void logPrgStateExec(PrgState prgState) {
        System.out.println(prgState.toString());
    }
}
