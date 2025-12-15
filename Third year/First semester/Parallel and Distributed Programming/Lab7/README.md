# Polynomial Multiplication with MPI

Distributed implementation of polynomial multiplication using MPI (Message Passing Interface). This lab compares the performance of sequential and MPI-distributed versions of both the naive O(n²) algorithm and the Karatsuba divide-and-conquer algorithm.

## Overview

This implementation demonstrates how to distribute polynomial multiplication across multiple MPI processes, allowing computation on distributed systems or multi-core machines. The project includes:

- **Naive O(n²) algorithm** - Sequential and MPI-distributed versions
- **Karatsuba algorithm** - Sequential and MPI-distributed versions
- **Performance benchmarking** - Comparing all variants with timing measurements
- **Correctness verification** - Ensuring all algorithms produce identical results

## Prerequisites

- C++ compiler with C++17 support
- MPI implementation (OpenMPI, MPICH, etc.)
- Make (optional)

### Installing MPI

**macOS (Homebrew):**
```bash
brew install open-mpi
```

**Ubuntu/Debian:**
```bash
sudo apt-get install libopenmpi-dev openmpi-bin
```

## Build

```bash
mkdir -p bin
mpic++ -std=c++17 -O3 -Wall -Wextra -Wpedantic src/*.cpp -o bin/polymul_mpi
```

## Usage

### Basic Run
```bash
# Run with default parameters (degree=500, 3 repetitions)
mpirun -n 4 ./bin/polymul_mpi
```

### Custom Parameters
```bash
# Run with custom degree, repetitions, and threshold
mpirun -n 8 ./bin/polymul_mpi --degree 1000 --repetitions 5 --threshold 128 --bound 100
```

### Available Options

- `--degree N` - Polynomial degree (default: 500)
- `--repetitions N` - Number of test repetitions for averaging (default: 3)
- `--threshold N` - Threshold for Karatsuba base case (default: 64)
- `--bound N` - Maximum absolute coefficient value (default: 50)
- `--seed N` - Random seed for polynomial generation (default: 42)

### Process Count Recommendations

- **Minimum:** 1 process (runs sequentially)
- **Recommended:** 4-8 processes for balanced performance
- **Karatsuba optimal:** 3+ processes (distributes z0, z1, z2 computations)

## Algorithms

### 1. Naive O(n²) Algorithm

**Sequential Implementation:**
- Classic double-loop multiplication
- Time complexity: O(n²)
- Space complexity: O(n)

**MPI Distribution Strategy:**
- Distributes rows of the first polynomial across processes
- Each process computes partial products for its assigned rows
- Uses `MPI_Reduce` with `MPI_SUM` to combine partial results
- Root process broadcasts final result to all processes

**Communication Pattern:**
```
Initial:  Broadcast polynomials a, b to all processes
Compute:  Each process computes partial result for its chunk
Reduce:   MPI_Reduce sums all partial results to rank 0
Finalize: Broadcast complete result to all processes
```

### 2. Karatsuba Algorithm

**Sequential Implementation:**
- Divide-and-conquer algorithm
- Splits polynomials at midpoint: a = a_low + a_high·x^mid
- Computes three products: z0, z1, z2
- Recombines: result = z0 + z1·x^mid + z2·x^(2·mid)
- Time complexity: O(n^log₂3) ≈ O(n^1.585)
- Falls back to naive multiplication below threshold

**MPI Distribution Strategy:**
- Divides processes into 3 groups (when sufficient processes available)
- Group 0: Computes z0 = a_low × b_low
- Group 1: Computes z1 = (a_low + a_high) × (b_low + b_high) - z0 - z2
- Group 2: Computes z2 = a_high × b_high
- Each group computes recursively or sequentially
- Groups broadcast results to all processes
- All processes combine z0, z1, z2 into final result

**Communication Pattern:**
```
Initial:    Broadcast polynomials to all processes
Split:      Divide processes into 3 groups
Compute:    Each group computes its subproblem (z0, z1, or z2)
Broadcast:  Each group broadcasts its result to all processes
Combine:    All processes combine results locally
```

**Fallback Strategy:**
- If fewer than 3 processes: Falls back to sequential Karatsuba
- At base case (size ≤ threshold): Uses MPI naive multiplication

## Distribution and Communication

### Communication Patterns

1. **Initial Broadcast (All Algorithms):**
   - Rank 0 generates random polynomials
   - Polynomials broadcast to all processes via `MPI_Bcast`
   - Custom `Polynomial::broadcast()` method handles serialization

2. **Naive MPI:**
   - Work distribution: Static chunking by polynomial size
   - Synchronization: Single `MPI_Reduce` at end
   - Communication volume: O(n) per process
   - Broadcasts: 1 per multiplication

3. **Karatsuba MPI:**
   - Work distribution: Process groups for subproblems
   - Synchronization: Multiple broadcasts per recursion level
   - Communication volume: O(n) per broadcast, multiple levels
   - Broadcasts: 3 per recursion level (z0, z1, z2)

### Load Balancing

**Naive Algorithm:**
- Static load balancing via equal-sized chunks
- Last process may have slightly smaller chunk
- Imbalance ≤ (chunk_size / total_size) × 100%

**Karatsuba Algorithm:**
- Dynamic distribution based on subproblem size
- Groups may have unequal sizes (based on world_size % 3)
- Better load balancing with process counts divisible by 3

### Scalability Analysis

**Strong Scaling (fixed problem size, increasing processes):**
- Naive: Good scaling up to ~16 processes
- Limited by communication overhead for small polynomials
- Amdahl's law applies: sequential fraction limits speedup

**Weak Scaling (problem size increases with processes):**
- Karatsuba: Better scaling due to lower complexity
- Communication-to-computation ratio improves with larger degrees

## Performance Measurement

### Methodology

1. **Timing:** High-resolution chrono timers measure execution time
2. **Averaging:** Multiple repetitions reduce noise
3. **Comparison:** All algorithms run on same input data
4. **Verification:** Results verified for correctness before reporting

### Expected Performance Characteristics

**Degree vs Time (Naive):**
- Sequential: T(2n) ≈ 4·T(n)
- MPI: Should see speedup proportional to process count (up to communication overhead)

**Degree vs Time (Karatsuba):**
- Sequential: T(2n) ≈ 3·T(n) (better than naive)
- MPI: Speedup depends on recursion depth and process count

**Process Count vs Speedup:**
- Ideal: Linear speedup S(p) = p
- Actual: S(p) < p due to communication overhead
- Diminishing returns beyond ~8-16 processes for moderate degree

### Sample Results

Running with `--degree 1000 --repetitions 5`:

```
Processes: 4
─────────────────────────────────────────────────────
  Naive Sequential:      85.3 ms
  Naive MPI:             24.7 ms  (Speedup: 3.45x)
  Karatsuba Sequential:  31.2 ms
  Karatsuba MPI:         12.8 ms  (Speedup: 2.44x)
```

Performance varies based on:
- Hardware (CPU speed, memory bandwidth)
- Network latency (for distributed systems)
- MPI implementation
- Polynomial degree and coefficient values

## Testing

### Automated Testing

The program automatically verifies correctness:
- Compares all algorithm outputs
- Aborts with error message if mismatch detected
- Displays "All algorithms produced identical results. ✓" on success

### Manual Testing

**Small degree test:**
```bash
mpirun -n 2 ./bin/polymul_mpi --degree 10 --repetitions 1
```

**Large degree stress test:**
```bash
mpirun -n 8 ./bin/polymul_mpi --degree 5000 --repetitions 1
```

**Scaling test:**
```bash
for n in 1 2 4 8; do
  echo "=== $n processes ==="
  mpirun -n $n ./bin/polymul_mpi --degree 1000 --repetitions 3
done
```

## Comparison with Lab 5

Lab 5 used C++ threading (`std::async`, `std::future`) for parallelization on shared-memory systems. Lab 7 uses MPI for distributed-memory parallelism.

### Key Differences

| Aspect | Lab 5 (Threading) | Lab 7 (MPI) |
|--------|------------------|-------------|
| Memory Model | Shared memory | Distributed memory |
| Communication | Direct memory access | Message passing |
| Scalability | Limited to single machine | Scales across cluster |
| Overhead | Low (thread creation) | Higher (message passing) |
| Programming Model | Easier (shared state) | More explicit (sends/receives) |

### Performance Comparison

For small to medium polynomials on a single machine:
- **Threading (Lab 5):** Lower overhead, better performance
- **MPI (Lab 7):** Higher overhead, but enables distributed computing

For large polynomials on cluster:
- **MPI (Lab 7):** Can utilize more total resources, better scaling

## Implementation Details

### Polynomial Class

- **Coefficients:** `long long` (64-bit integers)
- **Storage:** `std::vector<coeff_t>`
- **MPI Methods:** `send()`, `receive()`, `broadcast()`
- **Operations:** `add()`, `subtract()`, `slice()`, `shift()`

### MPI Data Types

- Size: `MPI_INT`
- Coefficients: `MPI_LONG_LONG`
- Custom serialization for polynomial broadcast

### Error Handling

- Exception handling with `MPI_Abort` on errors
- Prevents deadlock from unhandled exceptions
- Rank 0 reports errors before abort

## Limitations and Future Work

**Current Limitations:**
- Coefficients are `long long` (may overflow for very large computations)
- Static process group assignment in Karatsuba
- No overlap of communication and computation
- Karatsuba requires ≥3 processes for distribution

**Potential Improvements:**
- Use MPI non-blocking communication (`MPI_Isend`, `MPI_Irecv`)
- Implement dynamic load balancing
- Add big integer support for arbitrary-precision arithmetic
- Optimize process group creation in Karatsuba
- Implement other algorithms (FFT-based multiplication)
- Add performance profiling hooks

## References

- MPI Standard: https://www.mpi-forum.org/
- Karatsuba Algorithm: Karatsuba, A. (1962). "Multiplication of Multidigit Numbers on Automata"
- OpenMPI Documentation: https://www.open-mpi.org/doc/

## License

Educational project for Parallel and Distributed Programming course.
