import java.util.*;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicReference;

public class HamiltonianCycleFinder {
    private final Graph graph;
    private final int startVertex;
    private final AtomicReference<List<Integer>> solution;

    public HamiltonianCycleFinder(Graph graph, int startVertex) {
        this.graph = graph;
        this.startVertex = startVertex;
        this.solution = new AtomicReference<>(null);
    }

    private boolean isValidPath(List<Integer> path, int vertex) {
        return !path.contains(vertex);
    }

    private List<Integer> searchSequential(List<Integer> path) {
        if (solution.get() != null) {
            return null;
        }

        if (path.size() == graph.getNumVertices()) {
            List<Integer> neighbors = graph.getNeighbors(path.get(path.size() - 1));
            if (neighbors.contains(startVertex)) {
                List<Integer> cycle = new ArrayList<>(path);
                cycle.add(startVertex);
                solution.compareAndSet(null, cycle);
                return cycle;
            }
            return null;
        }

        int currentVertex = path.get(path.size() - 1);
        List<Integer> neighbors = graph.getNeighbors(currentVertex);

        for (int neighbor : neighbors) {
            if (solution.get() != null) {
                return null;
            }

            if (isValidPath(path, neighbor)) {
                List<Integer> newPath = new ArrayList<>(path);
                newPath.add(neighbor);
                List<Integer> result = searchSequential(newPath);
                if (result != null) {
                    return result;
                }
            }
        }

        return null;
    }

    public List<Integer> findSequential() {
        solution.set(null);
        List<Integer> path = new ArrayList<>();
        path.add(startVertex);
        searchSequential(path);
        return solution.get();
    }

    public List<Integer> findParallel(int parallelism) {
        solution.set(null);
        
        if (parallelism <= 1) {
            return findSequential();
        }

        ForkJoinPool pool = new ForkJoinPool(parallelism);
        try {
            List<Integer> path = new ArrayList<>();
            path.add(startVertex);
            
            HamiltonianTask task = new HamiltonianTask(path, parallelism);
            pool.invoke(task);
            
            return solution.get();
        } finally {
            pool.shutdown();
        }
    }

    private class HamiltonianTask extends RecursiveTask<List<Integer>> {
        private final List<Integer> path;
        private final int availableThreads;

        public HamiltonianTask(List<Integer> path, int availableThreads) {
            this.path = new ArrayList<>(path);
            this.availableThreads = availableThreads;
        }

        @Override
        protected List<Integer> compute() {
            if (solution.get() != null) {
                return null;
            }

            if (path.size() == graph.getNumVertices()) {
                List<Integer> neighbors = graph.getNeighbors(path.get(path.size() - 1));
                if (neighbors.contains(startVertex)) {
                    List<Integer> cycle = new ArrayList<>(path);
                    cycle.add(startVertex);
                    solution.compareAndSet(null, cycle);
                    return cycle;
                }
                return null;
            }

            int currentVertex = path.get(path.size() - 1);
            List<Integer> neighbors = graph.getNeighbors(currentVertex);
            
            List<Integer> validNeighbors = new ArrayList<>();
            for (int neighbor : neighbors) {
                if (isValidPath(path, neighbor)) {
                    validNeighbors.add(neighbor);
                }
            }

            if (validNeighbors.isEmpty()) {
                return null;
            }

            if (availableThreads > 1 && validNeighbors.size() > 1) {
                int threadsPerBranch = availableThreads / validNeighbors.size();
                int extraThreads = availableThreads % validNeighbors.size();

                List<HamiltonianTask> tasks = new ArrayList<>();
                
                for (int i = 0; i < validNeighbors.size(); i++) {
                    if (solution.get() != null) {
                        break;
                    }

                    int neighbor = validNeighbors.get(i);
                    List<Integer> newPath = new ArrayList<>(path);
                    newPath.add(neighbor);
                    
                    int branchThreads = threadsPerBranch + (i < extraThreads ? 1 : 0);
                    
                    if (branchThreads > 0) {
                        HamiltonianTask task = new HamiltonianTask(newPath, branchThreads);
                        tasks.add(task);
                        task.fork();
                    }
                }

                for (HamiltonianTask task : tasks) {
                    List<Integer> result = task.join();
                    if (result != null) {
                        return result;
                    }
                }
            } else {
                for (int neighbor : validNeighbors) {
                    if (solution.get() != null) {
                        return null;
                    }

                    List<Integer> newPath = new ArrayList<>(path);
                    newPath.add(neighbor);
                    List<Integer> result = searchSequential(newPath);
                    if (result != null) {
                        return result;
                    }
                }
            }

            return null;
        }
    }

    public void reset() {
        solution.set(null);
    }
}
