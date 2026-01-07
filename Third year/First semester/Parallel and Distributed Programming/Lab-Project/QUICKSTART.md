# Quick Start Guide

This guide will help you quickly build and test the Hough Transform implementation.

## Prerequisites

Make sure you have the following installed:
- C++ compiler with C++17 support (g++ or clang++)
- CMake 3.10 or higher
- OpenCV 4.x
- MPI implementation (OpenMPI or MPICH)
- Python 3 (for generating test images)

### Installing Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install build-essential cmake libopencv-dev libopenmpi-dev openmpi-bin python3
```

**macOS:**
```bash
brew install cmake opencv open-mpi python3
```

## Building the Project

1. Make the build script executable:
```bash
chmod +x test_build.sh
```

2. Run the build script:
```bash
./test_build.sh
```

This will create two executables in the `build/` directory:
- `hough_threaded` - Multi-threaded version
- `hough_mpi` - MPI distributed version

## Quick Test

### Generate Test Images

```bash
python3 generate_test_images.py
```

This creates several test images in the `test_images/` directory.

### Run Threaded Version

Detect lines:
```bash
./build/hough_threaded test_images/lines.png output/threaded/lines --threads 4 --mode lines
```

Detect circles:
```bash
./build/hough_threaded test_images/circles.png output/threaded/circles --threads 4 --mode circles --min-radius 30 --max-radius 110
```

Detect both:
```bash
./build/hough_threaded test_images/mixed.png output/threaded/mixed --threads 4 --mode both
```

### Run MPI Version

Detect lines:
```bash
mpirun -np 4 ./build/hough_mpi test_images/lines.png output/mpi/lines --mode lines
```

Detect circles:
```bash
mpirun -np 4 ./build/hough_mpi test_images/circles.png output/mpi/circles --mode circles --min-radius 30 --max-radius 110
```

Detect both:
```bash
mpirun -np 4 ./build/hough_mpi test_images/mixed.png output/mpi/mixed --mode both
```

## Running All Tests

Make the test script executable and run it:
```bash
chmod +x run_tests.sh
./run_tests.sh
```

This will run a comprehensive test suite on various images with both implementations.

## Performance Benchmarking

To benchmark performance scaling:
```bash
chmod +x benchmark.sh
./benchmark.sh
```

This will test both implementations with varying numbers of threads/processes and report timing results.

## Output Files

Each run produces:
- `edges.png` - Edge-detected image (Canny edges)
- `result.png` - Original image with detected lines/circles drawn
- `lines.txt` or `circles.txt` - Detected parameters (rho, theta, votes for lines; x, y, radius, votes for circles)
- `timing.txt` - Performance metrics (preprocessing time, Hough time, total time)

## Command-Line Options

### Threaded Version
```
./build/hough_threaded <input_image> <output_dir> [options]

Options:
  --threads <num>          Number of threads (default: hardware concurrency)
  --mode <lines|circles|both>  Detection mode (default: both)
  --threshold <value>      Detection threshold (default: 100)
  --min-radius <value>     Minimum circle radius (default: 10)
  --max-radius <value>     Maximum circle radius (default: 100)
```

### MPI Version
```
mpirun -np <num_processes> ./build/hough_mpi <input_image> <output_dir> [options]

Options:
  --mode <lines|circles|both>  Detection mode (default: both)
  --threshold <value>      Detection threshold (default: 100)
  --min-radius <value>     Minimum circle radius (default: 10)
  --max-radius <value>     Maximum circle radius (default: 100)
```

## Tips

1. **Threshold tuning**: Lower thresholds detect more shapes but may include false positives. Start with 100 and adjust.

2. **Circle radius range**: Set appropriate min/max radius based on expected circle sizes in your image.

3. **Thread count**: For threaded version, use the number of physical CPU cores for best performance.

4. **MPI processes**: For MPI version, use 2-8 processes on a single machine. For clusters, match the number of nodes.

5. **Image preprocessing**: The implementation includes automatic grayscale conversion and Canny edge detection.

## Troubleshooting

**Build fails with OpenCV not found:**
- Make sure OpenCV is installed
- Try: `export OpenCV_DIR=/path/to/opencv/build` before running cmake

**Build fails with MPI not found:**
- Install MPI: `sudo apt-get install libopenmpi-dev openmpi-bin`
- Or on macOS: `brew install open-mpi`

**No shapes detected:**
- Try lowering the threshold value
- Check that the input image has clear edges
- Verify the radius range for circles

**Poor performance:**
- Increase number of threads/processes
- Reduce the radius range for circle detection
- Use smaller images for testing

## Next Steps

- Try your own images
- Experiment with different thresholds
- Compare performance between threaded and MPI versions
- Analyze the timing results for different parallelization strategies

For more details, see the full README.md file.

