# Hough Transform - Parallel Implementation

This project implements the Hough transform for line detection with two parallel approaches: multi-threading and MPI distributed computing.

## Features

- **Line Detection**: Standard Hough transform for detecting straight lines
- **Parallel Processing**: Both preprocessing (grayscale conversion, edge detection) and Hough transform are parallelized
- **Two Implementations**: 
  - Multi-threaded (C++11 threads) for shared-memory systems
  - MPI distributed for multi-node clusters

## Requirements

- C++17 compiler (g++, clang++)
- CMake 3.10 or higher
- OpenCV 4.x
- OpenMPI or MPICH (for MPI version)

## Installation

### Install Dependencies (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install build-essential cmake
sudo apt-get install libopencv-dev
sudo apt-get install libopenmpi-dev openmpi-bin
```

### Install Dependencies (macOS)
```bash
brew install cmake
brew install opencv
brew install open-mpi
```

## Build

```bash
mkdir build
cd build
cmake ..
make
```

This will create two executables:
- `hough_threaded` - Multi-threaded version
- `hough_mpi` - MPI distributed version

## Usage

### Threaded Version

```bash
./hough_threaded <input_image> <output_dir> [OPTIONS]

Options:
  --threads <num>          Number of threads (default: hardware concurrency)
  --threshold <value>      Detection threshold (default: 100)
```

Example:
```bash
./hough_threaded ../test_images/sample.jpg ../output --threads 8
```

### MPI Version

```bash
mpirun -np <num_processes> ./hough_mpi <input_image> <output_dir> [OPTIONS]

Options:
  --threshold <value>      Detection threshold (default: 100)
```

Example:
```bash
mpirun -np 4 ./hough_mpi ../test_images/sample.jpg ../output
```

## Output

Both programs generate:
1. `edges.png` - Edge-detected image
2. `result.png` - Annotated image with detected lines
3. `lines.txt` - List of detected lines with parameters
4. `timing.txt` - Performance metrics (preprocessing, Hough transform, total time)

## Algorithm Overview

### Preprocessing Pipeline
1. **Load Image**: Read input image using OpenCV
2. **Grayscale Conversion**: Convert to grayscale (parallelized by rows)
3. **Edge Detection**: Apply Canny edge detector (parallelized by regions)

### Line Detection
- Parameter space: (ρ, θ) where ρ is distance from origin, θ is angle
- For each edge pixel, vote in accumulator space for all possible lines
- Find peaks in accumulator to detect lines

## Performance

Typical speedup with parallelization:
- **Threaded** (8 cores): 5-7x speedup over serial
- **MPI** (4 nodes): 3-4x speedup over serial

Performance depends on:
- Image size
- Number of edge pixels
- Detection parameters
- Hardware configuration

## Project Structure

```
Lab-Project/
├── CMakeLists.txt
├── README.md
├── include/
│   ├── hough_common.h
│   ├── hough_line.h
│   └── image_processor.h
├── src/
│   ├── common/
│   │   ├── hough_line.cpp
│   │   ├── hough_common.cpp
│   │   └── image_processor.cpp
│   ├── threaded/
│   │   ├── main_threaded.cpp
│   │   ├── parallel_processor.cpp
│   │   └── hough_line_threaded.cpp
│   └── mpi/
│       ├── main_mpi.cpp
│       ├── parallel_processor.cpp
│       └── hough_line_mpi.cpp
├── test_images/
└── output/
```

## License

MIT License

