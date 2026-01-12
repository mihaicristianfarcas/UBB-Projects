#!/bin/bash
# Benchmark script comparing different polynomial sizes

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
EXECUTABLE="$SCRIPT_DIR/build/polymul_metal"

if [ ! -f "$EXECUTABLE" ]; then
    echo "Error: Executable not found. Run ./build.sh first."
    exit 1
fi

echo "=== Polynomial Multiplication Benchmark ==="
echo "Machine: $(uname -m)"
echo "Date: $(date)"
echo ""

# Test different polynomial degrees
DEGREES=(100 500 1000 2000 5000 10000)
THRESHOLD=256
REPS=5

for degree in "${DEGREES[@]}"; do
    echo ""
    echo "========================================"
    echo "Testing degree: $degree"
    echo "========================================"
    "$EXECUTABLE" --degree "$degree" --threshold "$THRESHOLD" --repetitions "$REPS"
done

echo ""
echo "=== Benchmark Complete ==="
