#!/bin/bash
# Build script for the Hough Transform project

echo "Building Hough Transform Project"
echo "================================="

# Create build directory
mkdir -p build
cd build

# Configure with CMake
echo "Configuring with CMake..."
cmake .. || { echo "CMake configuration failed"; exit 1; }

# Build
echo "Building..."
# Detect number of CPUs (cross-platform)
if command -v nproc &> /dev/null; then
    NCPUS=$(nproc)
elif command -v sysctl &> /dev/null; then
    NCPUS=$(sysctl -n hw.ncpu)
else
    NCPUS=4
fi
make -j$NCPUS || { echo "Build failed"; exit 1; }

echo ""
echo "Build successful!"
echo ""
echo "Executables created:"
echo "  - build/hough_threaded"
echo "  - build/hough_mpi"
echo ""
echo "To run:"
echo "  ./build/hough_threaded <input> <output_dir> --threads 4 --mode both"
echo "  mpirun -np 4 ./build/hough_mpi <input> <output_dir> --mode both"

