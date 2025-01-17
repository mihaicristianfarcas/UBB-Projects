package Controller;

import Model.PrgState;
import Model.Utils.MyIHeap;
import Model.Values.RefValue;
import Model.Values.Value;
import Repository.MyIRepository;
import java.io.IOException;
import java.util.*;
import java.util.concurrent.*;
import java.util.stream.Collectors;

public class MyController {
    private final MyIRepository repository;
    private ExecutorService executor;
    private boolean displayFlag = false;

    public MyController(MyIRepository repository) {
        this.repository = repository;
    }

    public void addProgram(PrgState program) {
        repository.addProgram(program);
    }

    private List<Integer> getAllAddresses(Collection<Value> symTableValues, MyIHeap<Value> heapTable) {

        List<Integer> symTableAddresses = symTableValues.stream()
                .filter(v -> v instanceof RefValue)
                .map(v -> ((RefValue) v).getAddress())
                .toList();

        Queue<Integer> addressQueue = new LinkedList<>(symTableAddresses);
        Set<Integer> allAddresses = new HashSet<>(symTableAddresses);

        while (!addressQueue.isEmpty()) {
            int address = addressQueue.poll();
            Value value = heapTable.getContent().get(address);
            if (value instanceof RefValue refValue) {
                int newAddress = refValue.getAddress();
                if (allAddresses.add(newAddress)) {
                    addressQueue.add(newAddress);
                }
            }
        }

        return new ArrayList<>(allAddresses);
    }

    private Map<Integer, Value> safeGarbageCollector(List<Integer> symTableAddr, Map<Integer, Value> heap) {
        return heap.entrySet().stream()
                .filter(entry -> symTableAddr.contains(entry.getKey()))
                .collect(Collectors.toMap(Map.Entry::getKey, Map.Entry::getValue));
    }

    private void oneStepForAllPrg(List<PrgState> prgStates) throws InterruptedException {

        List<Callable<PrgState>> callList = prgStates.stream()
                .map(prg -> (Callable<PrgState>) prg::oneStep)
                .collect(Collectors.toList());

        List<PrgState> newPrgStates = executor.invokeAll(callList).stream()
                .map(future -> {
                    try {
                        return future.get();
                    } catch (Exception e) {
                        throw new RuntimeException("Error during program execution", e);
                    }
                })
                .filter(Objects::nonNull)
                .toList();

        prgStates.addAll(newPrgStates);

        // TODO not writing
        if (displayFlag) {
            prgStates.forEach(prg -> {
                try {
                    repository.logPrgStateExec(prg);
                } catch (IOException e) {
                    throw new RuntimeException("Error logging program state", e);
                }
            });
        }

        repository.setPrgList(prgStates);
    }

    public void allStep() throws InterruptedException {
        executor = Executors.newFixedThreadPool(2);

        List<PrgState> prgList = removeCompletedPrg(repository.getPrgList());

        while (!prgList.isEmpty()) {
            oneStepForAllPrg(prgList);

            // conservative garbage collector
            prgList.forEach(prg -> {
                List<Integer> referencedAddresses = getAllAddresses(
                        prg.getSymTable().getAllValues(),
                        prg.getHeap()
                );
                prg.getHeap().setContent(safeGarbageCollector(referencedAddresses, prg.getHeap().getContent()));
            });

            prgList = removeCompletedPrg(repository.getPrgList());
        }

        executor.shutdownNow();
        repository.setPrgList(prgList);
    }

    public List<PrgState> removeCompletedPrg(List<PrgState> inPrgList) {
        return inPrgList.stream()
                .filter(PrgState::isNotCompleted)
                .collect(Collectors.toList());
    }

    public void setDisplayFlag() {
        displayFlag = true;
    }

    public void runTypeChecker() throws Exception {
        repository.runTypeChecker();
    }
}
