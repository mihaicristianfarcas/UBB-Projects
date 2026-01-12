/**
 * Distributed Shared Memory (DSM) Library
 * 
 * This library provides a distributed shared memory mechanism with:
 * - Write operations to local or remote variables
 * - Callbacks for variable changes with total ordering guarantee
 * - Compare-and-exchange atomic operations
 * 
 * Uses MPI for inter-process communication.
 */

#ifndef DSM_H
#define DSM_H

#include <mpi.h>
#include <functional>
#include <map>
#include <set>
#include <vector>
#include <mutex>
#include <queue>
#include <thread>
#include <atomic>
#include <condition_variable>

namespace dsm {

// Message types for DSM protocol
enum class MessageType : int {
    WRITE_REQUEST = 1,      // Request to write a value
    WRITE_ACK = 2,          // Acknowledgment of write
    WRITE_COMMIT = 3,       // Commit the write (broadcast to all subscribers)
    CAS_REQUEST = 4,        // Compare-and-swap request
    CAS_RESPONSE = 5,       // Compare-and-swap response
    SHUTDOWN = 99           // Shutdown signal
};

// Structure representing a variable change event
struct ChangeEvent {
    int variableId;
    int oldValue;
    int newValue;
    int sourceRank;
    long long logicalTimestamp;  // For total ordering
};

// Callback type for variable change notifications
using ChangeCallback = std::function<void(const ChangeEvent&)>;

/**
 * DSM Class - Distributed Shared Memory Manager
 * 
 * Each process creates one instance of this class.
 * Variables are identified by integer IDs.
 * Only subscribers of a variable can read/write it.
 */
class DSM {
public:
    /**
     * Initialize the DSM system
     * @param argc Command line argument count
     * @param argv Command line arguments
     */
    static void init(int* argc, char*** argv);
    
    /**
     * Finalize the DSM system
     */
    static void finalize();
    
    /**
     * Get the rank of current process
     */
    static int getRank();
    
    /**
     * Get the total number of processes
     */
    static int getSize();
    
    /**
     * Constructor
     * @param numVariables Number of variables to manage
     * @param subscriptions Map of variable ID to set of subscriber ranks
     */
    DSM(int numVariables, const std::map<int, std::set<int>>& subscriptions);
    
    /**
     * Destructor - stops the listener thread
     */
    ~DSM();
    
    /**
     * Subscribe to change notifications
     * @param callback Function to call when any subscribed variable changes
     */
    void setChangeCallback(ChangeCallback callback);
    
    /**
     * Read a variable's current value
     * @param varId Variable ID
     * @return Current value of the variable
     * @throws runtime_error if not subscribed to the variable
     */
    int read(int varId);
    
    /**
     * Write a value to a variable
     * All subscribers will be notified in the same order.
     * @param varId Variable ID
     * @param value New value to write
     * @throws runtime_error if not subscribed to the variable
     */
    void write(int varId, int value);
    
    /**
     * Compare and exchange operation
     * Atomically: if variable == expected, set variable = newValue
     * @param varId Variable ID
     * @param expected Expected current value
     * @param newValue New value to set if comparison succeeds
     * @return true if exchange was performed, false otherwise
     * @throws runtime_error if not subscribed to the variable
     */
    bool compareAndExchange(int varId, int expected, int newValue);
    
    /**
     * Start the message listener thread
     */
    void start();
    
    /**
     * Stop the DSM and cleanup
     */
    void close();
    
    /**
     * Wait for all pending operations to complete
     */
    void synchronize();

private:
    int numVariables_;
    std::map<int, std::set<int>> subscriptions_;  // varId -> set of subscriber ranks
    std::map<int, int> variables_;                 // varId -> current value
    std::map<int, int> coordinators_;              // varId -> coordinator rank (lowest subscriber)
    
    ChangeCallback changeCallback_;
    
    std::thread listenerThread_;
    std::atomic<bool> running_;
    
    std::mutex variableMutex_;
    std::mutex callbackMutex_;
    
    // Logical clock for total ordering
    std::atomic<long long> logicalClock_;
    
    // Pending operations tracking
    std::map<long long, std::pair<int, int>> pendingWrites_;  // timestamp -> (varId, value)
    std::mutex pendingMutex_;
    std::condition_variable pendingCv_;
    std::set<long long> completedWrites_;
    
    // For CAS operations
    std::mutex casMutex_;
    std::condition_variable casCv_;
    std::map<long long, bool> casResults_;
    
    /**
     * Check if this process is subscribed to a variable
     */
    bool isSubscribed(int varId) const;
    
    /**
     * Get the coordinator for a variable (lowest-ranked subscriber)
     */
    int getCoordinator(int varId) const;
    
    /**
     * Listener thread function - handles incoming messages
     */
    void listenerLoop();
    
    /**
     * Handle a write request (coordinator only)
     */
    void handleWriteRequest(int sourceRank, int varId, int value, long long timestamp);
    
    /**
     * Handle a write commit message
     */
    void handleWriteCommit(int varId, int value, long long commitTimestamp, int sourceRank, long long requestTimestamp);
    
    /**
     * Handle a CAS request (coordinator only)
     */
    void handleCasRequest(int sourceRank, int varId, int expected, int newValue, long long timestamp);
    
    /**
     * Handle a CAS response
     */
    void handleCasResponse(long long timestamp, bool success);
    
    /**
     * Broadcast a message to all subscribers of a variable
     */
    void broadcastToSubscribers(int varId, MessageType type, const std::vector<int>& data);
    
    /**
     * Send a message to a specific rank
     */
    void sendMessage(int destRank, MessageType type, const std::vector<int>& data);
    
    /**
     * Increment and get the logical clock
     */
    long long getNextTimestamp();
    
    /**
     * Update logical clock on message receive
     */
    void updateClock(long long receivedTimestamp);
};

} // namespace dsm

#endif // DSM_H
