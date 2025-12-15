import java.util.*;

public class Main {
    
    private static void printCycle(List<Integer> cycle) {
        if (cycle == null || cycle.isEmpty()) {
            System.out.println("No Hamiltonian cycle found.");
        } else {
            System.out.print("Hamiltonian cycle found: ");
            for (int i = 0; i < cycle.size(); i++) {
                System.out.print(cycle.get(i));
                if (i < cycle.size() - 1) {
                    System.out.print(" -> ");
                }
            }
            System.out.println();
        }
    }

    private static void runBenchmark(Graph graph, String graphName, int startVertex) {
        System.out.println("\n" + "=".repeat(60));
        System.out.println("Testing: " + graphName);
        System.out.println("=".repeat(60));
        graph.print();
        System.out.println();

        HamiltonianCycleFinder finder = new HamiltonianCycleFinder(graph, startVertex);

        long startTime = System.nanoTime();
        List<Integer> resultSeq = finder.findSequential();
        long endTime = System.nanoTime();
        long durationSeq = (endTime - startTime) / 1000;

        System.out.print("Sequential: ");
        printCycle(resultSeq);
        System.out.println("Time: " + durationSeq + " microseconds\n");

        int[] threadCounts = {2, 4, 8, 16};
        
        System.out.printf("%-12s%-20s%-15s%-15s%n", "Threads", "Time (us)", "Speedup", "Efficiency");
        System.out.println("-".repeat(60));

        for (int numThreads : threadCounts) {
            finder.reset();
            
            startTime = System.nanoTime();
            List<Integer> resultPar = finder.findParallel(numThreads);
            endTime = System.nanoTime();
            long durationPar = (endTime - startTime) / 1000;

            double speedup = durationSeq == 0 ? 0 : (double) durationSeq / durationPar;
            double efficiency = speedup / numThreads * 100;

            System.out.printf("%-12d%-20d%-15.2f%-15.2f%%%n", 
                numThreads, durationPar, speedup, efficiency);

            if (resultSeq != null && !resultSeq.equals(resultPar) && resultPar != null) {
                System.out.print("  Result: ");
                printCycle(resultPar);
            }
        }
    }

    public static void main(String[] args) {
        System.out.println("HAMILTONIAN CYCLE FINDER - JAVA FORKJOINPOOL IMPLEMENTATION");
        System.out.println("Using ForkJoinPool and RecursiveTask for parallelization\n");

        runBenchmark(Graph.createSampleGraph1(), "Sample Graph 1 (5 vertices)", 0);
        runBenchmark(Graph.createSampleGraph2(), "Sample Graph 2 (6 vertices)", 0);
        runBenchmark(Graph.createNoHamiltonianGraph(), "No Hamiltonian Cycle Graph (4 vertices)", 0);
        
        System.out.println("\n" + "=".repeat(60));
        System.out.println("PERFORMANCE TESTING ON LARGER GRAPHS");
        System.out.println("=".repeat(60));
        
        runBenchmark(Graph.createLargerGraph(10), "Larger Graph (10 vertices)", 0);
        runBenchmark(Graph.createLargerGraph(12), "Larger Graph (12 vertices)", 0);
    }
}
