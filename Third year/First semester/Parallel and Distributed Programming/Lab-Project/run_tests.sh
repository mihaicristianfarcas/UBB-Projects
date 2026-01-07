#!/bin/bash
# Test script for the Hough Transform project

echo "Testing Hough Transform Implementation"
echo "======================================"

# Check if executables exist
if [ ! -f "build/hough_threaded" ]; then
    echo "Error: build/hough_threaded not found. Please run test_build.sh first."
    exit 1
fi

if [ ! -f "build/hough_mpi" ]; then
    echo "Error: build/hough_mpi not found. Please run test_build.sh first."
    exit 1
fi

# Generate test images
echo ""
echo "Generating test images..."
python3 generate_test_images.py || { echo "Failed to generate test images"; exit 1; }

# Create output directories
mkdir -p output/threaded
mkdir -p output/mpi

# Test 1: Lines detection with threaded version
echo ""
echo "Test 1: Line detection (Threaded, 4 threads)"
echo "---------------------------------------------"
./build/hough_threaded test_images/lines.png output/threaded/lines --threads 4 --mode lines --threshold 100

# Test 2: Circles detection with threaded version
echo ""
echo "Test 2: Circle detection (Threaded, 4 threads)"
echo "-----------------------------------------------"
./build/hough_threaded test_images/circles.png output/threaded/circles --threads 4 --mode circles --threshold 50 --min-radius 30 --max-radius 110

# Test 3: Mixed detection with threaded version
echo ""
echo "Test 3: Mixed detection (Threaded, 4 threads)"
echo "----------------------------------------------"
./build/hough_threaded test_images/mixed.png output/threaded/mixed --threads 4 --mode both --threshold 80

# Test 4: Lines detection with MPI version
echo ""
echo "Test 4: Line detection (MPI, 4 processes)"
echo "------------------------------------------"
mpirun -np 4 ./build/hough_mpi test_images/lines.png output/mpi/lines --mode lines --threshold 100

# Test 5: Circles detection with MPI version
echo ""
echo "Test 5: Circle detection (MPI, 4 processes)"
echo "--------------------------------------------"
mpirun -np 4 ./build/hough_mpi test_images/circles.png output/mpi/circles --mode circles --threshold 50 --min-radius 30 --max-radius 110

# Test 6: Mixed detection with MPI version
echo ""
echo "Test 6: Mixed detection (MPI, 4 processes)"
echo "-------------------------------------------"
mpirun -np 4 ./build/hough_mpi test_images/mixed.png output/mpi/mixed --mode both --threshold 80

# Test 7: Complex image with threaded version
echo ""
echo "Test 7: Complex image (Threaded, 8 threads)"
echo "--------------------------------------------"
./build/hough_threaded test_images/complex.png output/threaded/complex --threads 8 --mode both --threshold 150

# Test 8: House scene with MPI version
echo ""
echo "Test 8: House scene (MPI, 2 processes)"
echo "---------------------------------------"
mpirun -np 2 ./build/hough_mpi test_images/house.png output/mpi/house --mode both --threshold 80

echo ""
echo "======================================"
echo "All tests completed!"
echo ""
echo "Results are in:"
echo "  - output/threaded/"
echo "  - output/mpi/"
echo ""
echo "Each test directory contains:"
echo "  - edges.png: Edge-detected image"
echo "  - result.png: Annotated result"
echo "  - lines.txt or circles.txt: Detection parameters"
echo "  - timing.txt: Performance metrics"

