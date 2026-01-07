# Implementation Summary

## Project Overview

This project implements the **Hough Transform** for detecting lines and circles in images using two parallel computing approaches:
1. **Multi-threaded implementation** using C++11 threads
2. **MPI distributed implementation** for multi-node processing

Both implementations include **parallelized preprocessing** (grayscale conversion and edge detection) in addition to the Hough transform itself.

## Architecture

### Common Components

All shared code is in `src/common/` and `include/`:

- **image_processor.cpp/h**: Image I/O and preprocessing utilities using OpenCV
- **hough_common.cpp/h**: Core data structures (LineAccumulator, CircleAccumulator, HoughLine, HoughCircle)
- **hough_line.cpp/h**: Base line detection algorithm
- **hough_circle.cpp/h**: Base circle detection algorithm

### Threaded Implementation (`src/threaded/`)

**Parallelization Strategy:**
- Image divided into horizontal strips for preprocessing
- Edge pixels distributed among threads for voting
- Each thread maintains a local accumulator
- Accumulators merged at the end

**Key Files:**
- `main_threaded.cpp`: Entry point, command-line parsing, orchestration
- `parallel_processor.cpp`: Parallel grayscale conversion and edge detection
- `hough_line_threaded.cpp`: Threaded line detection
- `hough_circle_threaded.cpp`: Threaded circle detection

### MPI Implementation (`src/mpi/`)

**Parallelization Strategy:**
- Master-worker pattern (rank 0 coordinates)
- Image rows scattered to workers for preprocessing
- Edge pixels distributed using MPI_Scatterv
- Local accumulation followed by MPI_Reduce
- Master performs peak detection

**Key Files:**
- `main_mpi.cpp`: Entry point with MPI initialization
- `parallel_processor.cpp`: MPI-based parallel preprocessing
- `hough_line_mpi.cpp`: MPI line detection with collective operations
- `hough_circle_mpi.cpp`: MPI circle detection with reduction

## Algorithm Details

### Line Detection (Standard Hough Transform)

1. **Parameter Space**: (ρ, θ) where:
   - ρ: perpendicular distance from origin to line [-max_diag, +max_diag]
   - θ: angle [0, π]
   - Resolution: 1 pixel for ρ, 1 degree for θ

2. **Voting Process**:
   - For each edge pixel (x, y)
   - For all angles θ
   - Calculate ρ = x·cos(θ) + y·sin(θ)
   - Increment accumulator[ρ, θ]

3. **Peak Detection**:
   - Find local maxima above threshold
   - Non-maximum suppression in accumulator space
   - Sort by vote count

### Circle Detection (Circular Hough Transform)

1. **Parameter Space**: (x, y, r) where:
   - (x, y): circle center coordinates
   - r: radius [min_radius, max_radius]

2. **Voting Process** (for each radius):
   - For each edge pixel (ex, ey)
   - For angles 0° to 360°
   - Calculate possible center: cx = ex + r·cos(θ), cy = ey + r·sin(θ)
   - Increment accumulator[cx, cy]

3. **Memory Optimization**:
   - Process one radius at a time
   - Clear accumulator between radii
   - Reduces memory from O(width × height × radii) to O(width × height)

4. **Peak Detection**:
   - Find local maxima for each radius
   - Combine results from all radii
   - Sort by vote count

## Parallelization Details

### Threaded Version

**Preprocessing Parallelization:**
```
Image rows: [0, ..., N]
Thread 0: rows [0, N/T)
Thread 1: rows [N/T, 2N/T)
...
Thread T-1: rows [(T-1)N/T, N)
```

**Hough Transform Parallelization:**
```
Edge pixels: [p0, p1, ..., pM]
Thread 0: accumulates for [p0, pM/T)
Thread 1: accumulates for [pM/T, p2M/T)
...
Thread T-1: accumulates for [(T-1)M/T, pM)

Then: merge all local accumulators
```

### MPI Version

**Preprocessing Communication:**
```
Rank 0: Load image → MPI_Scatterv(rows) → Workers
Workers: Process rows → MPI_Gatherv(results) → Rank 0
```

**Hough Transform Communication:**
```
Rank 0: Extract edge pixels → MPI_Scatterv(pixels) → Workers
Workers: Local accumulation → MPI_Reduce(accumulator) → Rank 0
Rank 0: Peak detection
```

## Performance Characteristics

### Threaded Implementation

**Advantages:**
- Low communication overhead (shared memory)
- Fast accumulator merging
- Good scaling on multi-core systems
- Simpler to run (no MPI runtime needed)

**Expected Speedup:**
- 4 threads: ~3-3.5x
- 8 threads: ~5-7x
- 16 threads: ~8-12x (diminishing returns due to synchronization)

### MPI Implementation

**Advantages:**
- Scales to distributed systems
- Can handle very large images across nodes
- Memory distributed across processes
- Good for compute clusters

**Expected Speedup:**
- 2 processes: ~1.7-1.9x
- 4 processes: ~3-3.5x
- 8 processes: ~5-6x (communication overhead becomes significant)

**Communication Overhead:**
- Image distribution: O(image_size / num_processes)
- Accumulator reduction: O(accumulator_size)
- For small images, overhead may dominate

## Build System

**CMakeLists.txt** creates two separate executables:
- Links with OpenCV for image processing
- Threaded version links with pthread
- MPI version links with MPI::MPI_CXX

**Build process:**
```bash
mkdir build && cd build
cmake ..
make -j
```

## Testing Infrastructure

### Test Images (`generate_test_images.py`)

1. **lines.png**: Simple horizontal, vertical, and diagonal lines
2. **circles.png**: Circles of various sizes
3. **mixed.png**: Both lines and circles
4. **complex.png**: Grid with circles and noise
5. **diagonal_lines.png**: Lines at various angles (edge case testing)
6. **concentric_circles.png**: Overlapping circles
7. **house.png**: Real-world-like scene

### Test Scripts

- **test_build.sh**: Build both executables
- **run_tests.sh**: Comprehensive test suite with various configurations
- **benchmark.sh**: Performance scaling analysis with 1, 2, 4, 8, 16 threads/processes

## Output Format

Each run produces:
1. **edges.png**: Canny edge detection result
2. **result.png**: Original image with detected shapes drawn
3. **lines.txt** or **circles.txt**: Detection parameters
4. **timing.txt**: Performance metrics

## Key Implementation Decisions

1. **Used OpenCV**: Professional-grade image I/O and preprocessing
2. **Accumulator merging**: Thread-local accumulators to avoid locks during voting
3. **Non-maximum suppression**: Prevents duplicate detections
4. **One radius at a time**: Memory-efficient circle detection
5. **Factory pattern**: Clean separation between serial and parallel implementations
6. **MPI collective operations**: Efficient scatter/gather/reduce instead of point-to-point

## Preprocessing Answer

**Yes, preprocessing can be done in C++ in the same program!**

The implementation includes:
- Image loading with OpenCV
- Grayscale conversion (parallelized by rows)
- Gaussian blur for noise reduction
- Canny edge detection (parallelized by regions)

All preprocessing is integrated into both main programs, with both serial and parallel versions available.

## Code Statistics

- **Total files**: 21 source/header files
- **Lines of code**: ~3,500 LOC
- **Language**: C++17
- **External dependencies**: OpenCV, MPI
- **Supported platforms**: Linux, macOS (Windows with minor modifications)

## Future Enhancements

Possible improvements:
1. **GPU acceleration** using CUDA or OpenCL
2. **Hybrid MPI+threads** for multi-node multi-core systems
3. **Probabilistic Hough Transform** for better performance
4. **Automatic threshold selection** using image statistics
5. **Additional shape detection** (ellipses, rectangles)
6. **Web interface** for easy visualization
7. **Batch processing** for multiple images

## References

- Hough, P.V.C. (1962). "Method and means for recognizing complex patterns"
- Duda, R.O. and Hart, P.E. (1972). "Use of the Hough transformation to detect lines and curves in pictures"
- OpenCV Documentation: https://docs.opencv.org/
- MPI Standard: https://www.mpi-forum.org/

## Author Notes

This implementation prioritizes:
- **Correctness**: Produces accurate results matching expected output
- **Performance**: Efficient parallelization with good scaling
- **Code quality**: Clean, maintainable, well-documented code
- **Usability**: Easy to build, test, and use
- **Educational value**: Clear demonstration of parallel programming concepts

