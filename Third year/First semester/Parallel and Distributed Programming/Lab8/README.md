# Lab 8: Distributed Shared Memory (DSM)

## Goal
Implementation of a simple distributed shared memory mechanism to deal with consistency issues in distributed programming.

## Overview

This implementation provides a DSM library that supports:
- **Write operations** to local or remote variables
- **Change callbacks** with total ordering guarantee (all processes see changes in the same order)
- **Compare-and-exchange (CAS)** atomic operations
- **Subscription-based** variable access (only subscribers can read/write)

## Architecture

### Design Decisions

1. **Coordinator-based Total Ordering**: Each variable has a coordinator (the lowest-ranked subscriber). All write operations go through the coordinator, which ensures total ordering by assigning logical timestamps.

2. **Lamport Logical Clocks**: Used to maintain causal ordering across processes.

3. **Subscription Model**: Variables are only accessible to subscribed processes. Messages are only exchanged between subscribers of a variable (no centralized server).

4. **MPI with Multithreading**: Uses MPI for inter-process communication with a dedicated listener thread for handling incoming messages.

### Message Types

| Type | Description |
|------|-------------|
| WRITE_REQUEST | Request to the coordinator to write a value |
| WRITE_COMMIT | Broadcast from coordinator to all subscribers |
| CAS_REQUEST | Compare-and-swap request to coordinator |
| CAS_RESPONSE | Result of CAS operation |
| SHUTDOWN | Signal to stop the listener thread |

### Total Ordering Guarantee

The total ordering is guaranteed through:
1. All writes go through the variable's coordinator
2. The coordinator processes requests sequentially
3. The coordinator assigns commit timestamps before broadcasting
4. All subscribers receive commits in the same order

## Files

- **dsm.h** - DSM library header with class declarations
- **dsm.cpp** - DSM library implementation
- **main.cpp** - Demonstration program showing all DSM features
- **CMakeLists.txt** - CMake build configuration

## Building

### Prerequisites

- CMake 3.10 or higher
- MPI implementation (OpenMPI, MPICH, etc.)
- C++17 compatible compiler

### On macOS

```bash
# Install OpenMPI if not already installed
brew install open-mpi

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make
```

### On Linux

```bash
# Install OpenMPI
sudo apt-get install libopenmpi-dev  # Debian/Ubuntu
# or
sudo yum install openmpi-devel       # CentOS/RHEL

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make
```

## Running

Run the demonstration with 4 processes:

```bash
mpirun -np 4 ./dsm_main
```

Or with a different number of processes (minimum 2):

```bash
mpirun -np 2 ./dsm_main
```

## Demonstration Program

The main program demonstrates:

### Test 1: Basic Write Operations
All processes write to a shared variable. Callbacks show the total ordering.

### Test 2: Group-specific Writes
Processes write to variables they're subscribed to. Only subscribers receive notifications.

### Test 3: Compare and Exchange
Multiple processes attempt CAS operations on the same variable. Only one succeeds per attempt, demonstrating atomicity.

### Test 4: Cross-group Communication
Processes in different groups communicate through shared variables.

## API Reference

### DSM Class

```cpp
// Initialize MPI and DSM
static void DSM::init(int* argc, char*** argv);

// Finalize DSM and MPI  
static void DSM::finalize();

// Get current process rank
static int DSM::getRank();

// Get total number of processes
static int DSM::getSize();

// Constructor
DSM(int numVariables, const std::map<int, std::set<int>>& subscriptions);

// Set callback for variable changes
void setChangeCallback(ChangeCallback callback);

// Read a variable (must be subscribed)
int read(int varId);

// Write a value to a variable (must be subscribed)
void write(int varId, int value);

// Atomic compare-and-exchange
bool compareAndExchange(int varId, int expected, int newValue);

// Start the message listener
void start();

// Stop and cleanup
void close();

// Barrier synchronization
void synchronize();
```

### ChangeEvent Structure

```cpp
struct ChangeEvent {
    int variableId;        // Which variable changed
    int oldValue;          // Previous value
    int newValue;          // New value
    int sourceRank;        // Which process made the change
    long long logicalTimestamp;  // For ordering
};
```

## Example Usage

```cpp
#include "dsm.h"
#include <iostream>

int main(int argc, char* argv[]) {
    dsm::DSM::init(&argc, &argv);
    
    int rank = dsm::DSM::getRank();
    
    // Define subscriptions
    std::map<int, std::set<int>> subs;
    subs[0] = {0, 1, 2, 3};  // All processes subscribe to var 0
    
    // Create DSM with 1 variable
    dsm::DSM dsm(1, subs);
    
    // Set callback
    dsm.setChangeCallback([](const dsm::ChangeEvent& e) {
        std::cout << "Variable " << e.variableId 
                  << " = " << e.newValue << std::endl;
    });
    
    dsm.start();
    dsm.synchronize();
    
    // Each process writes its rank
    dsm.write(0, rank);
    
    dsm.synchronize();
    dsm.close();
    dsm::DSM::finalize();
    
    return 0;
}
```

## Notes

- The implementation assumes non-faulty computers
- Subscriptions are static and known at startup
- The coordinator for each variable is the lowest-ranked subscriber
- MPI_THREAD_MULTIPLE support is required for full functionality
