#!/bin/bash
# Benchmark script to compare performance scaling

echo "Hough Transform Performance Benchmark"
echo "====================================="

# Check if executables exist
if [ ! -f "build/hough_threaded" ] || [ ! -f "build/hough_mpi" ]; then
    echo "Error: Executables not found. Please run test_build.sh first."
    exit 1
fi

# Generate test images if not present
if [ ! -f "test_images/complex.png" ]; then
    echo "Generating test images..."
    python3 generate_test_images.py
fi

# Create benchmark output directory
mkdir -p benchmark_results

TEST_IMAGE="test_images/complex.png"

echo ""
echo "Test image: $TEST_IMAGE"
echo ""

# Benchmark threaded version with different thread counts
echo "Benchmarking Threaded Implementation"
echo "-------------------------------------"

for threads in 1 2 4 8 16; do
    echo "Testing with $threads thread(s)..."
    output_dir="benchmark_results/threaded_${threads}"
    mkdir -p "$output_dir"
    
    ./build/hough_threaded "$TEST_IMAGE" "$output_dir" \
        --threads $threads --threshold 150
    
    if [ -f "$output_dir/timing.txt" ]; then
        echo "Results for $threads thread(s):"
        cat "$output_dir/timing.txt"
        echo ""
    fi
done

echo ""
echo "Benchmarking MPI Implementation"
echo "--------------------------------"

# Benchmark MPI version with different process counts
for procs in 1 2 4 8; do
    echo "Testing with $procs process(es)..."
    output_dir="benchmark_results/mpi_${procs}"
    mkdir -p "$output_dir"
    
    mpirun -np $procs ./build/hough_mpi "$TEST_IMAGE" "$output_dir" \
        --threshold 150
    
    if [ -f "$output_dir/timing.txt" ]; then
        echo "Results for $procs process(es):"
        cat "$output_dir/timing.txt"
        echo ""
    fi
done

echo ""
echo "======================================"
echo "Benchmark completed!"
echo ""
echo "Summary of results:"
echo ""

# Generate summary
echo "Threaded Implementation:"
echo "------------------------"
for threads in 1 2 4 8 16; do
    timing_file="benchmark_results/threaded_${threads}/timing.txt"
    if [ -f "$timing_file" ]; then
        total_time=$(grep "Total time:" "$timing_file" | awk '{print $3}')
        echo "$threads thread(s): ${total_time} ms"
    fi
done

echo ""
echo "MPI Implementation:"
echo "-------------------"
for procs in 1 2 4 8; do
    timing_file="benchmark_results/mpi_${procs}/timing.txt"
    if [ -f "$timing_file" ]; then
        total_time=$(grep "Total time:" "$timing_file" | awk '{print $3}')
        echo "$procs process(es): ${total_time} ms"
    fi
done

echo ""
echo "Detailed results are in benchmark_results/"

