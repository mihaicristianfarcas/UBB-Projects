# Polynomial Multiplication with Apple Metal GPU Acceleration

This project implements polynomial multiplication using both CPU and GPU (Apple Metal) implementations, comparing the naive O(n²) algorithm and the Karatsuba algorithm.

## Table of Contents

1. [Overview](#overview)
2. [Algorithms](#algorithms)
   - [Naive Algorithm](#naive-algorithm)
   - [Karatsuba Algorithm](#karatsuba-algorithm)
3. [Parallelization Strategies](#parallelization-strategies)
   - [CPU Parallelization](#cpu-parallelization)
   - [GPU Parallelization with Metal](#gpu-parallelization-with-metal)
4. [Synchronization](#synchronization)
5. [Build Instructions](#build-instructions)
6. [Usage](#usage)
7. [Performance Measurements](#performance-measurements)
8. [Project Structure](#project-structure)

## Overview

Polynomial multiplication is a fundamental operation in computer algebra with applications in:
- Signal processing (convolution)
- Cryptography
- Scientific computing
- Error-correcting codes

Given two polynomials:
- P(x) = a₀ + a₁x + a₂x² + ... + aₙxⁿ
- Q(x) = b₀ + b₁x + b₂x² + ... + bₘxᵐ

Their product R(x) = P(x) × Q(x) has degree n + m with coefficients:
- rₖ = Σᵢ₊ⱼ₌ₖ aᵢ × bⱼ

## Algorithms

### Naive Algorithm

The naive algorithm directly computes each coefficient of the result by iterating through all pairs of input coefficients:

```
for i = 0 to n:
    for j = 0 to m:
        result[i + j] += a[i] * b[j]
```

**Time Complexity**: O(n × m) ≈ O(n²) for equal-degree polynomials

**Space Complexity**: O(n + m)

**Advantages**:
- Simple to implement
- Low overhead for small polynomials
- Highly parallelizable (independent coefficient computations)

### Karatsuba Algorithm

The Karatsuba algorithm uses divide-and-conquer to reduce the number of multiplications:

Given polynomials A and B, split each into low and high halves at position m = n/2:
- A = A_low + x^m × A_high
- B = B_low + x^m × B_high

Instead of computing 4 sub-products (as in naive divide-and-conquer), Karatsuba computes only 3:
- z₀ = A_low × B_low
- z₂ = A_high × B_high  
- z₁ = (A_low + A_high) × (B_low + B_high) - z₀ - z₂

Result: z₀ + z₁ × x^m + z₂ × x^(2m)

**Time Complexity**: O(n^log₂(3)) ≈ O(n^1.585)

**Space Complexity**: O(n) with proper implementation

**Advantages**:
- Asymptotically faster for large polynomials
- Reduces from 4 to 3 recursive multiplications

## Parallelization Strategies

### CPU Parallelization

#### Naive Parallel (CPU)
- Divide the first polynomial's coefficient indices into chunks
- Each thread computes partial products for its assigned indices
- Synchronization: Implicit via `std::future` - no shared writes occur during computation
- Final reduction: Sequential accumulation of partial result vectors

```cpp
// Each thread computes: partial[i+j] += a[i] * b[j] for i in [start, end)
auto futures = std::vector<std::future<std::vector<coeff_t>>>();
for (chunk in chunks) {
    futures.push_back(std::async(compute_chunk, chunk));
}
// Reduce results
for (auto& future : futures) {
    aggregate(future.get());
}
```

#### Karatsuba Parallel (CPU)
- The three sub-problems (z₀, z₁, z₂) can run in parallel
- We run z₀ and z₂ asynchronously while computing z₁ in the current thread
- Depth limiting prevents thread explosion (max_depth ≈ log₂(num_threads))
- Below max depth, falls back to sequential Karatsuba

```cpp
if (depth > 0) {
    auto future_z0 = std::async(karatsuba_parallel, a_low, b_low, depth-1);
    auto future_z2 = std::async(karatsuba_parallel, a_high, b_high, depth-1);
    z1 = karatsuba_parallel(a_sum, b_sum, depth-1);
    z0 = future_z0.get();
    z2 = future_z2.get();
}
```

### GPU Parallelization with Metal

#### Metal Naive GPU
Each GPU thread computes one coefficient of the result polynomial:

```metal
kernel void naive_multiply(
    device const long* a,
    device const long* b,
    device long* result,
    uint tid [[thread_position_in_grid]]
) {
    // Thread tid computes result[tid]
    long sum = 0;
    for valid (i, j) where i + j == tid:
        sum += a[i] * b[j];
    result[tid] = sum;
}
```

**Key characteristics**:
- **Thread mapping**: 1 thread per output coefficient
- **No synchronization needed**: Each thread writes to a unique memory location
- **Memory access pattern**: Coalesced reads from both input polynomials
- **Parallelism**: result_size threads execute simultaneously

#### Metal Karatsuba GPU
The recursive Karatsuba structure runs on CPU, but leverages GPU for:

1. **Base-case multiplications**: When size ≤ threshold, use GPU naive multiply
2. **Element-wise operations**: Addition, subtraction (fully parallel)
3. **Final combination**: Merge z₀, z₁, z₂ with shifts

```cpp
// Recursive structure on CPU
z0 = karatsuba_gpu(a_low, b_low, threshold);      // GPU for base case
z2 = karatsuba_gpu(a_high, b_high, threshold);    // GPU for base case
a_sum = gpu_add(a_low, a_high);                   // GPU element-wise
b_sum = gpu_add(b_low, b_high);                   // GPU element-wise
z1_full = karatsuba_gpu(a_sum, b_sum, threshold);
z1 = gpu_subtract(gpu_subtract(z1_full, z0), z2); // GPU element-wise
result = gpu_combine(z0, z1, z2, mid);            // GPU combine with shifts
```

## Synchronization

### CPU Synchronization
| Component | Synchronization Method |
|-----------|----------------------|
| Naive Parallel | `std::future` - threads write to separate partial arrays |
| Karatsuba Parallel | `std::async` + `std::future::get()` - wait for sub-tasks |
| Reduction | Sequential after all futures complete |

**No locks required**: Each thread operates on independent data.

### GPU Synchronization
| Component | Synchronization Method |
|-----------|----------------------|
| Within kernel | None needed - threads write to disjoint locations |
| Between kernels | Implicit via `waitUntilCompleted` |
| CPU-GPU | `MTLCommandBuffer::waitUntilCompleted` |

**Metal synchronization model**:
- Command buffers are submitted to a command queue
- `commit()` schedules execution
- `waitUntilCompleted()` blocks until GPU finishes
- No explicit barriers needed within our kernels (no shared memory races)

## Build Instructions

### Prerequisites
- macOS with Apple Silicon or Intel Mac with Metal support
- Xcode Command Line Tools
- CMake 3.20+

### Building

```bash
cd Lab-Bonus
mkdir -p build && cd build
cmake ..
make
```

This will:
1. Compile the Metal shaders to a `.metallib` file
2. Build the C++/Objective-C++ application
3. Link against Metal and Foundation frameworks

### Alternative: Manual Build

```bash
# Compile Metal shaders
xcrun -sdk macosx metal -c shaders/polynomial.metal -o polynomial.air
xcrun -sdk macosx metallib polynomial.air -o default.metallib

# Compile application
clang++ -std=c++20 -O2 -Wall \
    -I include \
    -framework Metal -framework Foundation \
    -fobjc-arc \
    src/main.cpp src/polynomial.cpp src/cpu_multiplier.cpp src/metal_multiplier.mm \
    -o polymul_metal
```

## Usage

```bash
./polymul_metal [options]

Options:
  --degree N        Polynomial degree (default: 500)
  --repetitions N   Number of repetitions for averaging (default: 3)
  --threads N       Number of CPU threads (default: hardware_concurrency)
  --bound N         Coefficient magnitude bound (default: 50)
  --threshold N     Karatsuba threshold (default: 64)
  --seed N          Random seed (default: 42)
  --no-verify       Skip result verification
  --verbose, -v     Verbose output
  --help, -h        Show help
```

### Examples

```bash
# Default benchmark
./polymul_metal

# Large polynomial test
./polymul_metal --degree 5000 --repetitions 5

# Quick test with verification
./polymul_metal --degree 100 --repetitions 1 --verbose

# Performance-focused (skip verification)
./polymul_metal --degree 10000 --no-verify --repetitions 10
```

## Performance Measurements

### Benchmark Environment
- **Machine**: Apple M4 Pro (14 CPU cores)
- **Metal Device**: Apple M4 Pro GPU
- **OS**: macOS

### Actual Benchmark Results

#### Degree 2000 (threshold=64, 5 repetitions)
| Algorithm | Time (ms) | Speedup vs Naive Seq |
|-----------|-----------|---------------------|
| CPU Naive Sequential | 6.37 | 1.00x |
| CPU Naive Parallel (14 threads) | 1.23 | 5.19x |
| CPU Karatsuba Sequential | 1.77 | 3.60x |
| CPU Karatsuba Parallel | 1.38 | 4.62x |
| **Metal GPU Naive** | **1.21** | **5.26x** |
| Metal GPU Karatsuba | 100.12 | 0.06x |

#### Degree 5000 (threshold=256, 3 repetitions)
| Algorithm | Time (ms) | Speedup vs Naive Seq |
|-----------|-----------|---------------------|
| CPU Naive Sequential | 40.16 | 1.00x |
| CPU Naive Parallel (14 threads) | 4.87 | 8.25x |
| CPU Karatsuba Sequential | 10.37 | 3.87x |
| CPU Karatsuba Parallel | 3.15 | 12.75x |
| **Metal GPU Naive** | **3.84** | **10.47x** |
| Metal GPU Karatsuba | 107.31 | 0.37x |

#### Degree 10000 (threshold=512, 3 repetitions)
| Algorithm | Time (ms) | Speedup vs Naive Seq |
|-----------|-----------|---------------------|
| CPU Naive Sequential | 159.07 | 1.00x |
| CPU Naive Parallel (14 threads) | 18.48 | 8.61x |
| CPU Karatsuba Sequential | 39.39 | 4.04x |
| CPU Karatsuba Parallel | 8.67 | 18.35x |
| **Metal GPU Naive** | **6.83** | **23.28x** |
| Metal GPU Karatsuba | 114.54 | 1.39x |

### Key Observations

1. **GPU Naive excels for the O(n²) algorithm**: For degree 10000, Metal GPU Naive achieves a **23.28x speedup** over CPU Naive Sequential, and is even faster than CPU Karatsuba Parallel (6.83ms vs 8.67ms).

2. **GPU overhead matters for small polynomials**: For degree < 500, CPU implementations are faster due to GPU memory transfer and kernel dispatch overhead.

3. **GPU Karatsuba has high dispatch overhead**: The recursive structure requires many GPU dispatches (for base cases and element-wise operations), which adds latency. A future optimization could batch multiple operations.

4. **CPU Karatsuba Parallel is efficient**: The thread-based parallel Karatsuba offers excellent performance for large polynomials without GPU overhead.

5. **Crossover point**: GPU acceleration becomes beneficial around degree 1000-2000 for the naive algorithm.

### Performance Characteristics

| Algorithm | Best For | Overhead |
|-----------|----------|----------|
| CPU Naive Sequential | Small polynomials (n < 100) | Minimal |
| CPU Naive Parallel | Medium polynomials with many cores | Thread creation |
| CPU Karatsuba Sequential | Large polynomials (n > 500) | Recursion |
| CPU Karatsuba Parallel | Very large polynomials | Thread + recursion |
| Metal GPU Naive | Any size with Metal | GPU setup, memory transfer |
| Metal GPU Karatsuba | Large polynomials | Multiple GPU dispatches |

### Factors Affecting GPU Performance

1. **Memory transfer overhead**: Copying data to/from GPU
2. **Kernel launch overhead**: Each dispatch has fixed cost
3. **Parallelism utilization**: Small polynomials may underutilize GPU
4. **Karatsuba recursion**: Multiple dispatches add latency

For the GPU Karatsuba implementation, the recursive structure means multiple GPU dispatches. For smaller polynomials, the GPU Naive implementation may actually outperform GPU Karatsuba due to lower dispatch overhead.

## Project Structure

```
Lab-Bonus/
├── CMakeLists.txt              # Build configuration
├── README.md                   # This documentation
├── build.sh                    # Build script
├── benchmark.sh                # Benchmark script (runs multiple sizes)
├── include/
│   ├── polynomial.hpp          # Polynomial class declaration
│   ├── cpu_multiplier.hpp      # CPU multiplication functions
│   └── metal_multiplier.hpp    # Metal GPU multiplication interface
├── src/
│   ├── main.cpp                # Benchmark program
│   ├── polynomial.cpp          # Polynomial class implementation
│   ├── cpu_multiplier.cpp      # CPU naive/Karatsuba implementations
│   └── metal_multiplier.mm     # Metal GPU implementation (Obj-C++)
├── shaders/
│   └── polynomial.metal        # Metal compute shaders
└── build/                      # Build output (created by cmake)
    ├── polymul_metal           # Executable
    └── shaders/                # Shader sources (for runtime compilation)
```

## Comparison with Lab5 CPU Implementation

This bonus lab extends Lab5 by adding Apple Metal GPU acceleration:

| Feature | Lab5 | Lab-Bonus |
|---------|------|-----------|
| Naive Sequential | ✓ | ✓ |
| Naive Parallel (threads) | ✓ | ✓ |
| Karatsuba Sequential | ✓ | ✓ |
| Karatsuba Parallel (threads) | ✓ | ✓ |
| GPU Naive (Metal) | ✗ | ✓ |
| GPU Karatsuba (Metal) | ✗ | ✓ |

The GPU implementations provide significant speedups for medium to large polynomials, particularly for the naive algorithm where the highly parallel nature maps well to GPU execution.
