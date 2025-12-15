#!/bin/bash

echo "=========================================="
echo "Hamiltonian Cycle Finder - Test Suite"
echo "=========================================="
echo ""

# Build and run C++ implementation
echo "1. Building C++ Implementation..."
echo "------------------------------------------"
make clean > /dev/null 2>&1
make
if [ $? -eq 0 ]; then
    echo "✓ C++ build successful"
    echo ""
    echo "2. Running C++ Implementation..."
    echo "------------------------------------------"
    ./hamiltonian_cpp
else
    echo "✗ C++ build failed"
fi

echo ""
echo ""
echo "=========================================="
echo ""

# Build and run Java implementation
echo "3. Compiling Java Implementation..."
echo "------------------------------------------"
rm -f *.class > /dev/null 2>&1
javac Graph.java HamiltonianCycleFinder.java Main.java
if [ $? -eq 0 ]; then
    echo "✓ Java compilation successful"
    echo ""
    echo "4. Running Java Implementation..."
    echo "------------------------------------------"
    java Main
else
    echo "✗ Java compilation failed"
fi

echo ""
echo "=========================================="
echo "Test suite completed!"
echo "=========================================="
