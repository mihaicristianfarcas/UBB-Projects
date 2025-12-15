#include "Graph.h"
#include "HamiltonianCycleFinder.h"
#include <iostream>
#include <chrono>
#include <iomanip>

void printCycle(const std::vector<int>& cycle) {
    if (cycle.empty()) {
        std::cout << "No Hamiltonian cycle found.\n";
    } else {
        std::cout << "Hamiltonian cycle found: ";
        for (size_t i = 0; i < cycle.size(); i++) {
            std::cout << cycle[i];
            if (i < cycle.size() - 1) std::cout << " -> ";
        }
        std::cout << "\n";
    }
}

void runBenchmark(const Graph& graph, const std::string& graphName, int startVertex = 0) {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "Testing: " << graphName << "\n";
    std::cout << std::string(60, '=') << "\n";
    graph.print();
    std::cout << "\n";

    HamiltonianCycleFinder finder(graph, startVertex);

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> resultSeq = finder.findSequential();
    auto end = std::chrono::high_resolution_clock::now();
    auto durationSeq = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Sequential: ";
    printCycle(resultSeq);
    std::cout << "Time: " << durationSeq << " microseconds\n\n";

    std::vector<int> threadCounts = {2, 4, 8, 16};
    
    std::cout << std::left << std::setw(12) << "Threads" 
              << std::setw(20) << "Time (us)" 
              << std::setw(15) << "Speedup" 
              << std::setw(15) << "Efficiency" << "\n";
    std::cout << std::string(60, '-') << "\n";

    for (int numThreads : threadCounts) {
        finder.reset();
        
        start = std::chrono::high_resolution_clock::now();
        std::vector<int> resultPar = finder.findParallel(numThreads);
        end = std::chrono::high_resolution_clock::now();
        auto durationPar = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        double speedup = static_cast<double>(durationSeq) / durationPar;
        double efficiency = speedup / numThreads * 100;

        std::cout << std::left << std::setw(12) << numThreads
                  << std::setw(20) << durationPar
                  << std::setw(15) << std::fixed << std::setprecision(2) << speedup
                  << std::setw(15) << std::fixed << std::setprecision(2) << efficiency << "%\n";

        if (!resultSeq.empty() && resultPar != resultSeq) {
            std::cout << "  Result: ";
            printCycle(resultPar);
        }
    }
}

Graph createLargerGraph(int size) {
    Graph g(size);
    
    for (int i = 0; i < size; i++) {
        g.addEdge(i, (i + 1) % size);
    }
    
    for (int i = 0; i < size; i++) {
        if (i + 2 < size) {
            g.addEdge(i, i + 2);
        }
        if (i > 1) {
            g.addEdge(i, i - 2);
        }
    }
    
    return g;
}

int main() {
    std::cout << "HAMILTONIAN CYCLE FINDER - C++ PARALLEL IMPLEMENTATION\n";
    std::cout << "Using std::thread for parallelization\n";

    runBenchmark(Graph::createSampleGraph1(), "Sample Graph 1 (5 vertices)", 0);
    runBenchmark(Graph::createSampleGraph2(), "Sample Graph 2 (6 vertices)", 0);
    runBenchmark(Graph::createNoHamiltonianGraph(), "No Hamiltonian Cycle Graph (4 vertices)", 0);
    
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "PERFORMANCE TESTING ON LARGER GRAPHS\n";
    std::cout << std::string(60, '=') << "\n";
    
    runBenchmark(createLargerGraph(10), "Larger Graph (10 vertices)", 0);
    runBenchmark(createLargerGraph(12), "Larger Graph (12 vertices)", 0);

    return 0;
}
