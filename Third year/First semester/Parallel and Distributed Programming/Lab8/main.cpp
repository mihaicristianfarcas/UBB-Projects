/**
 * Main Program - Distributed Shared Memory Demonstration
 * 
 * This program demonstrates the DSM library with:
 * - Multiple processes sharing variables
 * - Write operations with total ordering
 * - Compare-and-exchange operations
 * - Change callbacks
 * 
 * Run with: mpirun -np 4 ./dsm_main
 */

#include "dsm.h"
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <iomanip>

// Helper function for synchronized output
std::mutex printMutex;
void syncPrint(int rank, const std::string& message) {
    std::lock_guard<std::mutex> lock(printMutex);
    std::cout << "[Process " << rank << "] " << message << std::endl;
}

// Define the subscription configuration
// Variable 0: All processes (0, 1, 2, 3) - shared counter
// Variable 1: Processes 0, 1 - group A counter  
// Variable 2: Processes 2, 3 - group B counter
// Variable 3: Processes 0, 2 - cross-group variable
// Variable 4: Processes 1, 3 - cross-group variable
std::map<int, std::set<int>> createSubscriptions(int numProcesses) {
    std::map<int, std::set<int>> subs;
    
    // Variable 0: All processes subscribe
    for (int i = 0; i < numProcesses; ++i) {
        subs[0].insert(i);
    }
    
    // Variable 1: Processes 0 and 1
    subs[1].insert(0);
    subs[1].insert(1);
    
    // Variable 2: Processes 2 and 3
    subs[2].insert(2);
    subs[2].insert(3);
    
    // Variable 3: Processes 0 and 2
    subs[3].insert(0);
    subs[3].insert(2);
    
    // Variable 4: Processes 1 and 3
    subs[4].insert(1);
    subs[4].insert(3);
    
    return subs;
}

int main(int argc, char* argv[]) {
    // Initialize DSM/MPI
    dsm::DSM::init(&argc, &argv);
    
    int rank = dsm::DSM::getRank();
    int size = dsm::DSM::getSize();
    
    if (size < 2) {
        if (rank == 0) {
            std::cerr << "This program requires at least 2 processes." << std::endl;
            std::cerr << "Run with: mpirun -np 4 ./dsm_main" << std::endl;
        }
        dsm::DSM::finalize();
        return 1;
    }
    
    syncPrint(rank, "Starting DSM demonstration");
    
    // Create subscriptions
    auto subscriptions = createSubscriptions(size);
    
    // Create DSM instance with 5 variables
    const int NUM_VARIABLES = 5;
    dsm::DSM dsm(NUM_VARIABLES, subscriptions);
    
    // Set up change callback
    dsm.setChangeCallback([rank](const dsm::ChangeEvent& event) {
        std::ostringstream oss;
        oss << "CALLBACK: Variable " << event.variableId 
            << " changed from " << event.oldValue 
            << " to " << event.newValue
            << " (by process " << event.sourceRank 
            << ", timestamp " << event.logicalTimestamp << ")";
        syncPrint(rank, oss.str());
    });
    
    // Start the DSM listener
    dsm.start();
    
    // Synchronize all processes before starting
    dsm.synchronize();
    
    syncPrint(rank, "=== Starting Test 1: Basic Write Operations ===");
    
    // Test 1: Each process writes to variable 0 (all subscribed)
    // This tests basic write with total ordering
    std::this_thread::sleep_for(std::chrono::milliseconds(100 * rank));
    
    int writeValue = (rank + 1) * 10;
    syncPrint(rank, "Writing " + std::to_string(writeValue) + " to variable 0");
    dsm.write(0, writeValue);
    
    dsm.synchronize();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // Read and display current value
    int currentValue = dsm.read(0);
    syncPrint(rank, "Variable 0 current value: " + std::to_string(currentValue));
    
    dsm.synchronize();
    
    // Test 2: Group-specific writes
    syncPrint(rank, "=== Starting Test 2: Group-specific Writes ===");
    
    if (rank == 0 || rank == 1) {
        // Write to variable 1 (group A)
        int val = 100 + rank;
        syncPrint(rank, "Writing " + std::to_string(val) + " to variable 1 (group A)");
        dsm.write(1, val);
    }
    
    if (rank == 2 || rank == 3) {
        // Write to variable 2 (group B)
        int val = 200 + rank;
        syncPrint(rank, "Writing " + std::to_string(val) + " to variable 2 (group B)");
        dsm.write(2, val);
    }
    
    dsm.synchronize();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // Test 3: Compare and Exchange
    syncPrint(rank, "=== Starting Test 3: Compare and Exchange ===");
    dsm.synchronize();
    
    // Reset variable 0 to 0 from process 0
    if (rank == 0) {
        dsm.write(0, 0);
    }
    dsm.synchronize();
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    
    // All processes try to increment variable 0 using CAS
    // Only one should succeed at a time
    for (int attempt = 0; attempt < 3; ++attempt) {
        int expected = dsm.read(0);
        int desired = expected + 1;
        
        bool success = dsm.compareAndExchange(0, expected, desired);
        
        std::ostringstream oss;
        oss << "CAS attempt " << attempt << ": expected=" << expected 
            << ", desired=" << desired << ", result=" << (success ? "SUCCESS" : "FAILED");
        syncPrint(rank, oss.str());
        
        // Small delay to allow other processes to attempt
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    dsm.synchronize();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // Final value check
    currentValue = dsm.read(0);
    syncPrint(rank, "Final value of variable 0: " + std::to_string(currentValue));
    
    // Test 4: Cross-group communication
    syncPrint(rank, "=== Starting Test 4: Cross-group Communication ===");
    dsm.synchronize();
    
    if (rank == 0) {
        syncPrint(rank, "Writing 999 to variable 3 (shared with process 2)");
        dsm.write(3, 999);
    }
    
    if (rank == 1) {
        syncPrint(rank, "Writing 888 to variable 4 (shared with process 3)");
        dsm.write(4, 888);
    }
    
    dsm.synchronize();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // Read cross-group variables
    if (rank == 0 || rank == 2) {
        int val = dsm.read(3);
        syncPrint(rank, "Variable 3 value: " + std::to_string(val));
    }
    
    if (rank == 1 || rank == 3) {
        int val = dsm.read(4);
        syncPrint(rank, "Variable 4 value: " + std::to_string(val));
    }
    
    dsm.synchronize();
    
    syncPrint(rank, "=== DSM Demonstration Complete ===");
    
    // Cleanup
    dsm.close();
    dsm::DSM::finalize();
    
    return 0;
}
