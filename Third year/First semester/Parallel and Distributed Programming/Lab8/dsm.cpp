/**
 * Distributed Shared Memory (DSM) Library Implementation
 */

#include "dsm.h"
#include <iostream>
#include <algorithm>
#include <cstring>
#include <stdexcept>

namespace dsm {

// Static MPI state
static int s_rank = -1;
static int s_size = -1;
static bool s_initialized = false;

void DSM::init(int* argc, char*** argv) {
    if (!s_initialized) {
        int provided;
        MPI_Init_thread(argc, argv, MPI_THREAD_MULTIPLE, &provided);
        if (provided < MPI_THREAD_MULTIPLE) {
            std::cerr << "Warning: MPI does not provide full thread support. "
                      << "Provided level: " << provided << std::endl;
        }
        MPI_Comm_rank(MPI_COMM_WORLD, &s_rank);
        MPI_Comm_size(MPI_COMM_WORLD, &s_size);
        s_initialized = true;
    }
}

void DSM::finalize() {
    if (s_initialized) {
        MPI_Finalize();
        s_initialized = false;
    }
}

int DSM::getRank() {
    return s_rank;
}

int DSM::getSize() {
    return s_size;
}

DSM::DSM(int numVariables, const std::map<int, std::set<int>>& subscriptions)
    : numVariables_(numVariables)
    , subscriptions_(subscriptions)
    , running_(false)
    , logicalClock_(0)
{
    // Initialize all variables to 0
    for (int i = 0; i < numVariables_; ++i) {
        if (isSubscribed(i)) {
            variables_[i] = 0;
        }
    }
    
    // Determine coordinator for each variable (lowest-ranked subscriber)
    for (const auto& [varId, subscribers] : subscriptions_) {
        if (!subscribers.empty()) {
            coordinators_[varId] = *subscribers.begin();  // set is ordered, first is smallest
        }
    }
}

DSM::~DSM() {
    close();
}

void DSM::setChangeCallback(ChangeCallback callback) {
    std::lock_guard<std::mutex> lock(callbackMutex_);
    changeCallback_ = callback;
}

bool DSM::isSubscribed(int varId) const {
    auto it = subscriptions_.find(varId);
    if (it == subscriptions_.end()) return false;
    return it->second.count(s_rank) > 0;
}

int DSM::getCoordinator(int varId) const {
    auto it = coordinators_.find(varId);
    if (it == coordinators_.end()) {
        throw std::runtime_error("Variable " + std::to_string(varId) + " has no coordinator");
    }
    return it->second;
}

int DSM::read(int varId) {
    if (!isSubscribed(varId)) {
        throw std::runtime_error("Process " + std::to_string(s_rank) + 
                                 " is not subscribed to variable " + std::to_string(varId));
    }
    
    std::lock_guard<std::mutex> lock(variableMutex_);
    return variables_[varId];
}

void DSM::write(int varId, int value) {
    if (!isSubscribed(varId)) {
        throw std::runtime_error("Process " + std::to_string(s_rank) + 
                                 " is not subscribed to variable " + std::to_string(varId));
    }
    
    long long timestamp = getNextTimestamp();
    int coordinator = getCoordinator(varId);
    
    if (coordinator == s_rank) {
        // We are the coordinator - directly commit
        handleWriteRequest(s_rank, varId, value, timestamp);
    } else {
        // Send write request to coordinator
        std::vector<int> data = {varId, value, 
                                 static_cast<int>(timestamp & 0xFFFFFFFF),
                                 static_cast<int>((timestamp >> 32) & 0xFFFFFFFF)};
        sendMessage(coordinator, MessageType::WRITE_REQUEST, data);
        
        // Wait for commit
        std::unique_lock<std::mutex> lock(pendingMutex_);
        pendingCv_.wait(lock, [this, timestamp]() {
            return completedWrites_.count(timestamp) > 0 || !running_;
        });
        completedWrites_.erase(timestamp);
    }
}

bool DSM::compareAndExchange(int varId, int expected, int newValue) {
    if (!isSubscribed(varId)) {
        throw std::runtime_error("Process " + std::to_string(s_rank) + 
                                 " is not subscribed to variable " + std::to_string(varId));
    }
    
    long long timestamp = getNextTimestamp();
    int coordinator = getCoordinator(varId);
    
    if (coordinator == s_rank) {
        // We are the coordinator - handle locally
        std::lock_guard<std::mutex> lock(variableMutex_);
        if (variables_[varId] == expected) {
            // Perform the exchange
            int oldValue = variables_[varId];
            variables_[varId] = newValue;
            
            // Broadcast to other subscribers
            const auto& subscribers = subscriptions_[varId];
            for (int sub : subscribers) {
                if (sub != s_rank) {
                    std::vector<int> data = {varId, newValue, 
                                             static_cast<int>(timestamp & 0xFFFFFFFF),
                                             static_cast<int>((timestamp >> 32) & 0xFFFFFFFF),
                                             s_rank,
                                             static_cast<int>(timestamp & 0xFFFFFFFF),
                                             static_cast<int>((timestamp >> 32) & 0xFFFFFFFF)};
                    sendMessage(sub, MessageType::WRITE_COMMIT, data);
                }
            }
            
            // Notify callback
            {
                std::lock_guard<std::mutex> cbLock(callbackMutex_);
                if (changeCallback_) {
                    ChangeEvent event{varId, oldValue, newValue, s_rank, timestamp};
                    changeCallback_(event);
                }
            }
            
            return true;
        }
        return false;
    } else {
        // Send CAS request to coordinator
        std::vector<int> data = {varId, expected, newValue,
                                 static_cast<int>(timestamp & 0xFFFFFFFF),
                                 static_cast<int>((timestamp >> 32) & 0xFFFFFFFF)};
        sendMessage(coordinator, MessageType::CAS_REQUEST, data);
        
        // Wait for response
        std::unique_lock<std::mutex> lock(casMutex_);
        casCv_.wait(lock, [this, timestamp]() {
            return casResults_.count(timestamp) > 0 || !running_;
        });
        
        bool result = casResults_[timestamp];
        casResults_.erase(timestamp);
        return result;
    }
}

void DSM::start() {
    running_ = true;
    listenerThread_ = std::thread(&DSM::listenerLoop, this);
}

void DSM::close() {
    if (running_) {
        running_ = false;
        
        // Send shutdown to self to unblock listener
        std::vector<int> data;
        sendMessage(s_rank, MessageType::SHUTDOWN, data);
        
        if (listenerThread_.joinable()) {
            listenerThread_.join();
        }
    }
    
    // Wake up any waiting threads
    pendingCv_.notify_all();
    casCv_.notify_all();
}

void DSM::synchronize() {
    MPI_Barrier(MPI_COMM_WORLD);
}

void DSM::listenerLoop() {
    while (running_) {
        MPI_Status status;
        int flag;
        
        // Probe for incoming messages
        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
        
        if (flag) {
            int count;
            MPI_Get_count(&status, MPI_INT, &count);
            
            std::vector<int> buffer(count);
            MPI_Recv(buffer.data(), count, MPI_INT, status.MPI_SOURCE, 
                     status.MPI_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            MessageType type = static_cast<MessageType>(status.MPI_TAG);
            
            switch (type) {
                case MessageType::SHUTDOWN:
                    running_ = false;
                    break;
                    
                case MessageType::WRITE_REQUEST: {
                    // Format: [varId, value, timestamp_low, timestamp_high]
                    int varId = buffer[0];
                    int value = buffer[1];
                    long long timestamp = static_cast<long long>(buffer[2]) | 
                                         (static_cast<long long>(buffer[3]) << 32);
                    updateClock(timestamp);
                    handleWriteRequest(status.MPI_SOURCE, varId, value, timestamp);
                    break;
                }
                
                case MessageType::WRITE_COMMIT: {
                    // Format: [varId, value, commitTimestamp_low, commitTimestamp_high, sourceRank, requestTimestamp_low, requestTimestamp_high]
                    int varId = buffer[0];
                    int value = buffer[1];
                    long long commitTimestamp = static_cast<long long>(buffer[2]) | 
                                         (static_cast<long long>(buffer[3]) << 32);
                    int sourceRank = buffer[4];
                    long long requestTimestamp = static_cast<long long>(buffer[5]) | 
                                         (static_cast<long long>(buffer[6]) << 32);
                    updateClock(commitTimestamp);
                    handleWriteCommit(varId, value, commitTimestamp, sourceRank, requestTimestamp);
                    break;
                }
                
                case MessageType::CAS_REQUEST: {
                    // Format: [varId, expected, newValue, timestamp_low, timestamp_high]
                    int varId = buffer[0];
                    int expected = buffer[1];
                    int newValue = buffer[2];
                    long long timestamp = static_cast<long long>(buffer[3]) | 
                                         (static_cast<long long>(buffer[4]) << 32);
                    updateClock(timestamp);
                    handleCasRequest(status.MPI_SOURCE, varId, expected, newValue, timestamp);
                    break;
                }
                
                case MessageType::CAS_RESPONSE: {
                    // Format: [timestamp_low, timestamp_high, success]
                    long long timestamp = static_cast<long long>(buffer[0]) | 
                                         (static_cast<long long>(buffer[1]) << 32);
                    bool success = buffer[2] != 0;
                    handleCasResponse(timestamp, success);
                    break;
                }
                
                default:
                    std::cerr << "Unknown message type: " << static_cast<int>(type) << std::endl;
            }
        } else {
            // Small sleep to avoid busy waiting
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
    }
}

void DSM::handleWriteRequest(int sourceRank, int varId, int value, long long timestamp) {
    // This is called on the coordinator
    int oldValue;
    {
        std::lock_guard<std::mutex> lock(variableMutex_);
        oldValue = variables_[varId];
        variables_[varId] = value;
    }
    
    // Get a new timestamp for ordering
    long long commitTimestamp = getNextTimestamp();
    
    // Broadcast commit to all subscribers, including original request timestamp
    const auto& subscribers = subscriptions_[varId];
    for (int sub : subscribers) {
        if (sub != s_rank) {
            std::vector<int> data = {varId, value, 
                                     static_cast<int>(commitTimestamp & 0xFFFFFFFF),
                                     static_cast<int>((commitTimestamp >> 32) & 0xFFFFFFFF),
                                     sourceRank,
                                     static_cast<int>(timestamp & 0xFFFFFFFF),
                                     static_cast<int>((timestamp >> 32) & 0xFFFFFFFF)};
            sendMessage(sub, MessageType::WRITE_COMMIT, data);
        }
    }
    
    // Notify local callback
    {
        std::lock_guard<std::mutex> lock(callbackMutex_);
        if (changeCallback_) {
            ChangeEvent event{varId, oldValue, value, sourceRank, commitTimestamp};
            changeCallback_(event);
        }
    }
    
    // If the request came from ourselves, mark as complete
    if (sourceRank == s_rank) {
        std::lock_guard<std::mutex> lock(pendingMutex_);
        completedWrites_.insert(timestamp);
        pendingCv_.notify_all();
    }
}

void DSM::handleWriteCommit(int varId, int value, long long commitTimestamp, int sourceRank, long long requestTimestamp) {
    int oldValue;
    {
        std::lock_guard<std::mutex> lock(variableMutex_);
        oldValue = variables_[varId];
        variables_[varId] = value;
    }
    
    // Notify callback
    {
        std::lock_guard<std::mutex> lock(callbackMutex_);
        if (changeCallback_) {
            ChangeEvent event{varId, oldValue, value, sourceRank, commitTimestamp};
            changeCallback_(event);
        }
    }
    
    // If we were waiting for this write (we initiated it), mark as complete
    if (sourceRank == s_rank) {
        std::lock_guard<std::mutex> lock(pendingMutex_);
        completedWrites_.insert(requestTimestamp);
        pendingCv_.notify_all();
    }
}

void DSM::handleCasRequest(int sourceRank, int varId, int expected, int newValue, long long timestamp) {
    bool success = false;
    int oldValue;
    
    {
        std::lock_guard<std::mutex> lock(variableMutex_);
        if (variables_[varId] == expected) {
            oldValue = variables_[varId];
            variables_[varId] = newValue;
            success = true;
        }
    }
    
    if (success) {
        // Get commit timestamp
        long long commitTimestamp = getNextTimestamp();
        
        // Broadcast commit to all subscribers
        const auto& subscribers = subscriptions_[varId];
        for (int sub : subscribers) {
            if (sub != s_rank) {
                std::vector<int> data = {varId, newValue, 
                                         static_cast<int>(commitTimestamp & 0xFFFFFFFF),
                                         static_cast<int>((commitTimestamp >> 32) & 0xFFFFFFFF),
                                         sourceRank,
                                         static_cast<int>(timestamp & 0xFFFFFFFF),
                                         static_cast<int>((timestamp >> 32) & 0xFFFFFFFF)};
                sendMessage(sub, MessageType::WRITE_COMMIT, data);
            }
        }
        
        // Notify local callback
        {
            std::lock_guard<std::mutex> lock(callbackMutex_);
            if (changeCallback_) {
                ChangeEvent event{varId, oldValue, newValue, sourceRank, commitTimestamp};
                changeCallback_(event);
            }
        }
    }
    
    // Send response to requester
    std::vector<int> response = {
        static_cast<int>(timestamp & 0xFFFFFFFF),
        static_cast<int>((timestamp >> 32) & 0xFFFFFFFF),
        success ? 1 : 0
    };
    sendMessage(sourceRank, MessageType::CAS_RESPONSE, response);
}

void DSM::handleCasResponse(long long timestamp, bool success) {
    std::lock_guard<std::mutex> lock(casMutex_);
    casResults_[timestamp] = success;
    casCv_.notify_all();
}

void DSM::broadcastToSubscribers(int varId, MessageType type, const std::vector<int>& data) {
    const auto& subscribers = subscriptions_[varId];
    for (int sub : subscribers) {
        sendMessage(sub, type, data);
    }
}

void DSM::sendMessage(int destRank, MessageType type, const std::vector<int>& data) {
    MPI_Send(data.data(), static_cast<int>(data.size()), MPI_INT, 
             destRank, static_cast<int>(type), MPI_COMM_WORLD);
}

long long DSM::getNextTimestamp() {
    return ++logicalClock_;
}

void DSM::updateClock(long long receivedTimestamp) {
    long long current = logicalClock_.load();
    while (receivedTimestamp >= current) {
        if (logicalClock_.compare_exchange_weak(current, receivedTimestamp + 1)) {
            break;
        }
    }
}

} // namespace dsm
