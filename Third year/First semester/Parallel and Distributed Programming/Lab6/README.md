# Parallel Hamiltonian Cycle Finder

## Overview

This project implements a parallel algorithm to find Hamiltonian cycles in directed graphs. A Hamiltonian cycle is a path in a graph that visits each vertex exactly once and returns to the starting vertex.

Two implementations are provided:
1. **C++ Implementation** using `std::thread`
2. **Java Implementation** using `ForkJoinPool` and `RecursiveTask`

---

## Table of Contents

1. [Algorithm Description](#algorithm-description)
2. [Parallelization Strategy](#parallelization-strategy)
3. [Synchronization Mechanisms](#synchronization-mechanisms)
4. [Implementation Details](#implementation-details)
5. [Performance Analysis](#performance-analysis)
6. [Building and Running](#building-and-running)
7. [Results and Conclusions](#results-and-conclusions)

---

## Algorithm Description

### Sequential Algorithm

The base algorithm uses **backtracking** to explore all possible paths in the graph:

1. **Initialize**: Start with a path containing only the start vertex
2. **Check completion**: If the path contains all vertices:
   - Check if there's an edge from the last vertex back to the start vertex
   - If yes, we found a Hamiltonian cycle
3. **Explore neighbors**: For each unvisited neighbor of the current vertex:
   - Add the neighbor to the path
   - Recursively continue the search
   - If no solution found, backtrack (remove the neighbor from the path)
4. **Return**: Return the first valid cycle found, or null if none exists

**Time Complexity**: O(n!) in the worst case, where n is the number of vertices  
**Space Complexity**: O(n) for the path storage and recursion stack

### Parallel Algorithm

The parallelization strategy divides work at multiple levels of the search tree:

1. **Start from fixed vertex**: Begin exploration from vertex 0 (configurable)
2. **Multi-level work distribution**:
   - At the first level, distribute neighbors of the start vertex among threads
   - At subsequent levels, continue subdividing work if threads are available
   - When no more threads available, fall back to sequential search

3. **Thread allocation strategy**:
   - Given N threads and M branches (neighbors):
     - Base threads per branch: `threadsPerBranch = N / M`
     - Extra threads: `extraThreads = N % M`
     - First `extraThreads` branches get `threadsPerBranch + 1` threads
     - Remaining branches get `threadsPerBranch` threads

**Example with 8 threads and 3 neighbors at start vertex**:
- Branch 1: 3 threads (8/3 = 2, plus 1 extra)
- Branch 2: 3 threads (plus 1 extra)
- Branch 3: 2 threads (base allocation)

---

## Parallelization Strategy

### Work Distribution

The parallelization happens at **multiple levels** of the search tree:

```
                    Start (0)
                   /    |    \
                  /     |     \
           [3 threads] [3 threads] [2 threads]
                /       |       \
               1        2        3
              / \      / \      / \
           [1t][2t] [1t][2t] [1t][1t]
           ...      ...       ...
```

### Key Design Decisions

1. **Early termination**: Once any thread finds a solution, all threads check a shared flag and stop exploring
2. **Work stealing**: Not explicitly implemented, but the recursive nature allows natural load balancing
3. **Granularity control**: Parallel splitting stops when thread count becomes too small (< 2)
4. **Memory efficiency**: Each parallel branch gets its own path copy to avoid synchronization overhead

### Thread Management

**C++ (`std::thread`)**:
- Explicit thread creation and management
- Threads created for each major branch
- Join all threads before returning results

**Java (`ForkJoinPool`)**:
- Work-stealing thread pool
- Recursive task decomposition using `RecursiveTask<List<Integer>>`
- Automatic load balancing by the framework
- Fork-join pattern for parallel exploration

---

## Synchronization Mechanisms

### C++ Implementation

1. **Atomic Flag** (`std::atomic<bool> solutionFound`):
   - Shared flag indicating if a solution has been found
   - Atomic operations ensure thread-safe access without locks
   - Used for early termination of all threads

2. **Mutex** (`std::mutex solutionMutex`):
   - Protects the solution vector when a thread finds a cycle
   - Only locked briefly to copy the solution
   - Uses `std::lock_guard` for RAII-style locking

3. **Thread Joining**:
   - Main thread waits for all worker threads to complete
   - Ensures all exploration is finished before returning

**Synchronization Flow**:
```cpp
// Thread checks atomic flag before continuing
if (solutionFound.load()) {
    return false;
}

// Thread attempts to store solution atomically
if (!solutionFound.exchange(true)) {
    std::lock_guard<std::mutex> lock(solutionMutex);
    solution = path;
    solution.push_back(startVertex);
}
```

### Java Implementation

1. **AtomicReference** (`AtomicReference<List<Integer>> solution`):
   - Thread-safe reference to the solution
   - Uses `compareAndSet` for lock-free updates
   - Ensures only one thread stores the solution

2. **ForkJoinPool**:
   - Manages thread pool automatically
   - Work-stealing scheduler for load balancing
   - Handles thread synchronization internally

3. **RecursiveTask**:
   - Provides fork-join semantics
   - `fork()`: Asynchronously execute task
   - `join()`: Wait for task completion and get result

**Synchronization Flow**:
```java
// Check if solution already found
if (solution.get() != null) {
    return null;
}

// Atomically store solution
solution.compareAndSet(null, cycle);

// Fork-join pattern
task.fork();  // Start parallel execution
result = task.join();  // Wait for completion
```

---

## Implementation Details

### C++ Implementation

**Files**:
- `Graph.h`: Graph data structure with adjacency list
- `HamiltonianCycleFinder.h`: Parallel and sequential search algorithms
- `main.cpp`: Benchmarking and testing code
- `Makefile`: Build configuration

**Key Features**:
- Template-free for simplicity
- RAII-style resource management
- Move semantics for efficiency
- Clear separation between sequential and parallel logic

**Compilation**:
```bash
g++ -std=c++17 -pthread -O2 -Wall -Wextra main.cpp -o hamiltonian_cpp
```

### Java Implementation

**Files**:
- `Graph.java`: Graph data structure
- `HamiltonianCycleFinder.java`: Parallel and sequential search using ForkJoinPool
- `Main.java`: Benchmarking and testing code

**Key Features**:
- Clean object-oriented design
- Inner class for RecursiveTask implementation
- Automatic memory management
- Work-stealing for better load balancing

**Compilation**:
```bash
javac Graph.java HamiltonianCycleFinder.java Main.java
java Main
```

---

## Performance Analysis

### Test Environment

- **Processor**: (Varies by system - multi-core CPU)
- **Graphs tested**:
  - Small graphs: 4-6 vertices
  - Medium graphs: 10-12 vertices
  - Various connectivity patterns

### Performance Metrics

1. **Execution Time**: Measured in microseconds
2. **Speedup**: Sequential time / Parallel time
3. **Efficiency**: (Speedup / Number of threads) × 100%

### Expected Results

**Small Graphs (4-6 vertices)**:
- Sequential is often faster due to overhead
- Parallel overhead dominates computation time
- Speedup < 1.0 in most cases

**Larger Graphs (10+ vertices)**:
- Better parallelization opportunities
- Improved speedup as problem size increases
- Still limited by early solution finding

### Performance Observations

1. **Thread Overhead**: For small graphs, thread creation and synchronization overhead exceeds computation time
2. **Early Termination Impact**: Finding solution early limits parallelization benefits
3. **Load Imbalance**: Different branches have different exploration depths
4. **Scalability**: Performance improves with larger, more complex graphs

### Typical Results (C++ Implementation)

```
Graph: 12 vertices
Sequential: 1 microsecond
2 threads:  37 microseconds  (Speedup: 0.03)
4 threads:  61 microseconds  (Speedup: 0.02)
8 threads:  129 microseconds (Speedup: 0.01)
```

The overhead is evident in small graphs. For truly large graphs (20+ vertices) with no easy solutions, parallel benefits become more apparent.

### Typical Results (Java Implementation)

```
Graph: 5 vertices
Sequential: 144 microseconds
2 threads:  938 microseconds  (Speedup: 0.15)
4 threads:  198 microseconds  (Speedup: 0.73)
8 threads:  160 microseconds  (Speedup: 0.90)
```

Java shows better scalability due to ForkJoinPool's work-stealing, but still affected by small problem sizes.

---

## Building and Running

### C++ Implementation

```bash
# Build
make

# Run
./hamiltonian_cpp

# Clean
make clean
```

**Requirements**:
- C++17 compatible compiler
- pthread library
- Make (optional, can compile directly with g++)

### Java Implementation

```bash
# Compile
javac Graph.java HamiltonianCycleFinder.java Main.java

# Run
java Main

# Clean
rm -f *.class
```

**Requirements**:
- Java 8 or higher (for ForkJoinPool)
- JDK with javac compiler

---

## Results and Conclusions

### Algorithm Effectiveness

Both implementations successfully find Hamiltonian cycles when they exist and correctly identify when no cycle exists.

### Parallelization Insights

1. **Overhead vs. Benefit**: For small graphs, parallelization overhead outweighs benefits
2. **Multi-level Distribution**: The multi-level work distribution strategy works as designed
3. **Early Termination**: Critical for avoiding wasted computation
4. **Thread Allocation**: Dynamic thread distribution ensures all cores are utilized

### When to Use Parallel Version

The parallel implementation is beneficial when:
- Graph has many vertices (20+)
- Graph is sparse (many dead-ends to explore)
- No easy solution exists (requires extensive search)
- Running on multi-core system with available cores

### Comparison: C++ vs. Java

**C++**:
- ✅ Lower overhead
- ✅ More control over thread management
- ✅ Better for performance-critical applications
- ❌ More complex to implement correctly
- ❌ Manual thread management

**Java**:
- ✅ Simpler code with ForkJoinPool
- ✅ Automatic work-stealing
- ✅ Built-in load balancing
- ❌ Higher overhead from JVM
- ❌ Less fine-grained control

### Future Improvements

1. **Dynamic granularity**: Adjust parallel depth based on graph properties
2. **Heuristic ordering**: Explore promising neighbors first
3. **Better load balancing**: Implement work-stealing in C++
4. **GPU parallelization**: Explore CUDA/OpenCL for massive parallelism
5. **Pruning strategies**: Use graph properties to eliminate branches early

### Conclusion

The parallel Hamiltonian cycle finder demonstrates:
- Multi-level work distribution strategy
- Effective synchronization for early termination
- Trade-offs between parallelization overhead and benefits
- Successful implementation in both C++ (threads) and Java (ForkJoin)

While small graphs don't benefit from parallelization, the infrastructure scales well and would show significant improvements on larger, more complex graphs where the search space is extensive.

---

## References

- **Hamiltonian Path Problem**: NP-complete problem in graph theory
- **Backtracking Algorithms**: Classic approach for constraint satisfaction problems
- **Fork-Join Framework**: Java's parallel computation framework
- **C++ Threading**: std::thread and synchronization primitives

---

## Author

Lab 6 - Parallel and Distributed Programming  
Implementation of Parallel Hamiltonian Cycle Finder

---

## License

Educational project for academic purposes.
