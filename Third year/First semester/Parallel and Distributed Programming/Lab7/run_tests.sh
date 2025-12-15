#!/bin/bash

# Comprehensive test suite for MPI polynomial multiplication
# Tests correctness, performance, and scaling

set -e  # Exit on error

BINARY="./bin/polymul_mpi"
RESULTS_FILE="test_results.txt"

echo "======================================================"
echo "  MPI Polynomial Multiplication - Test Suite"
echo "======================================================"
echo ""

# Check if binary exists
if [ ! -f "$BINARY" ]; then
    echo "Error: Binary not found. Please run 'make' first."
    exit 1
fi

# Check if MPI is available
if ! command -v mpirun &> /dev/null; then
    echo "Error: mpirun not found. Please install MPI."
    exit 1
fi

echo "Binary: $BINARY"
echo "Results will be saved to: $RESULTS_FILE"
echo ""

# Clear results file
> "$RESULTS_FILE"

echo "Starting tests..." | tee -a "$RESULTS_FILE"
echo "" | tee -a "$RESULTS_FILE"

# Test 1: Correctness test with small degree
echo "═══════════════════════════════════════════════════" | tee -a "$RESULTS_FILE"
echo "Test 1: Correctness Verification (degree=50)" | tee -a "$RESULTS_FILE"
echo "═══════════════════════════════════════════════════" | tee -a "$RESULTS_FILE"
mpirun -n 4 "$BINARY" --degree 50 --repetitions 1 2>&1 | tee -a "$RESULTS_FILE"
echo "" | tee -a "$RESULTS_FILE"

# Test 2: Performance with different degrees
echo "═══════════════════════════════════════════════════" | tee -a "$RESULTS_FILE"
echo "Test 2: Performance vs Degree (4 processes)" | tee -a "$RESULTS_FILE"
echo "═══════════════════════════════════════════════════" | tee -a "$RESULTS_FILE"

for degree in 100 200 500 1000; do
    echo "--- Degree: $degree ---" | tee -a "$RESULTS_FILE"
    mpirun -n 4 "$BINARY" --degree $degree --repetitions 3 2>&1 | tee -a "$RESULTS_FILE"
    echo "" | tee -a "$RESULTS_FILE"
done

# Test 3: Scaling test (strong scaling)
echo "═══════════════════════════════════════════════════" | tee -a "$RESULTS_FILE"
echo "Test 3: Strong Scaling (degree=1000, varying processes)" | tee -a "$RESULTS_FILE"
echo "═══════════════════════════════════════════════════" | tee -a "$RESULTS_FILE"

for procs in 1 2 4 8; do
    echo "--- $procs process(es) ---" | tee -a "$RESULTS_FILE"
    mpirun -n $procs "$BINARY" --degree 1000 --repetitions 5 2>&1 | tee -a "$RESULTS_FILE"
    echo "" | tee -a "$RESULTS_FILE"
done

# Test 4: Karatsuba threshold sensitivity
echo "═══════════════════════════════════════════════════" | tee -a "$RESULTS_FILE"
echo "Test 4: Threshold Sensitivity (degree=800, 4 processes)" | tee -a "$RESULTS_FILE"
echo "═══════════════════════════════════════════════════" | tee -a "$RESULTS_FILE"

for threshold in 32 64 128 256; do
    echo "--- Threshold: $threshold ---" | tee -a "$RESULTS_FILE"
    mpirun -n 4 "$BINARY" --degree 800 --threshold $threshold --repetitions 3 2>&1 | tee -a "$RESULTS_FILE"
    echo "" | tee -a "$RESULTS_FILE"
done

# Test 5: Edge cases
echo "═══════════════════════════════════════════════════" | tee -a "$RESULTS_FILE"
echo "Test 5: Edge Cases" | tee -a "$RESULTS_FILE"
echo "═══════════════════════════════════════════════════" | tee -a "$RESULTS_FILE"

echo "--- Very small degree (10) ---" | tee -a "$RESULTS_FILE"
mpirun -n 4 "$BINARY" --degree 10 --repetitions 1 2>&1 | tee -a "$RESULTS_FILE"
echo "" | tee -a "$RESULTS_FILE"

echo "--- Single process ---" | tee -a "$RESULTS_FILE"
mpirun -n 1 "$BINARY" --degree 200 --repetitions 2 2>&1 | tee -a "$RESULTS_FILE"
echo "" | tee -a "$RESULTS_FILE"

echo "--- Large number of processes (16) ---" | tee -a "$RESULTS_FILE"
mpirun -n 16 "$BINARY" --degree 500 --repetitions 2 2>&1 | tee -a "$RESULTS_FILE"
echo "" | tee -a "$RESULTS_FILE"

# Test 6: Stress test
echo "═══════════════════════════════════════════════════" | tee -a "$RESULTS_FILE"
echo "Test 6: Stress Test (degree=2000, 8 processes)" | tee -a "$RESULTS_FILE"
echo "═══════════════════════════════════════════════════" | tee -a "$RESULTS_FILE"
mpirun -n 8 "$BINARY" --degree 2000 --repetitions 1 2>&1 | tee -a "$RESULTS_FILE"
echo "" | tee -a "$RESULTS_FILE"

echo "======================================================"
echo "  All tests completed successfully!"
echo "  Results saved to: $RESULTS_FILE"
echo "======================================================"
