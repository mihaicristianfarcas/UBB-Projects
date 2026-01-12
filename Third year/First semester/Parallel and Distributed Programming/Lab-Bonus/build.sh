#!/bin/bash
# Build script for Lab-Bonus Polynomial Multiplication with Metal

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"

echo "=== Building Polynomial Multiplication with Apple Metal ==="
echo ""

# Create build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure with CMake
echo "Configuring with CMake..."
cmake ..

# Build
echo ""
echo "Building..."
make -j$(sysctl -n hw.ncpu)

echo ""
echo "=== Build Complete ==="
echo "Executable: $BUILD_DIR/polymul_metal"
echo ""
echo "Run with: ./build/polymul_metal --help"
