#ifndef HAMILTONIAN_CYCLE_FINDER_H
#define HAMILTONIAN_CYCLE_FINDER_H

#include "Graph.h"
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
#include <memory>

class HamiltonianCycleFinder {
private:
    const Graph& graph;
    std::atomic<bool> solutionFound;
    std::vector<int> solution;
    std::mutex solutionMutex;
    int startVertex;

    bool isValidPath(const std::vector<int>& path, int vertex) const {
        return std::find(path.begin(), path.end(), vertex) == path.end();
    }

    bool searchSequential(std::vector<int>& path) {
        if (solutionFound.load()) {
            return false;
        }

        if (path.size() == graph.getNumVertices()) {
            const std::vector<int>& neighbors = graph.getNeighbors(path.back());
            if (std::find(neighbors.begin(), neighbors.end(), startVertex) != neighbors.end()) {
                if (!solutionFound.exchange(true)) {
                    std::lock_guard<std::mutex> lock(solutionMutex);
                    solution = path;
                    solution.push_back(startVertex);
                    return true;
                }
            }
            return false;
        }

        int currentVertex = path.back();
        const std::vector<int>& neighbors = graph.getNeighbors(currentVertex);

        for (int neighbor : neighbors) {
            if (solutionFound.load()) {
                return false;
            }

            if (isValidPath(path, neighbor)) {
                path.push_back(neighbor);
                if (searchSequential(path)) {
                    return true;
                }
                path.pop_back();
            }
        }

        return false;
    }

    void searchParallel(std::vector<int> path, const std::vector<int>& neighbors, 
                       int startIdx, int endIdx, int threadsAvailable) {
        if (solutionFound.load()) {
            return;
        }

        for (int i = startIdx; i < endIdx && !solutionFound.load(); i++) {
            int neighbor = neighbors[i];
            
            if (isValidPath(path, neighbor)) {
                std::vector<int> newPath = path;
                newPath.push_back(neighbor);
                
                if (threadsAvailable > 1 && newPath.size() < graph.getNumVertices()) {
                    const std::vector<int>& nextNeighbors = graph.getNeighbors(neighbor);
                    std::vector<int> validNeighbors;
                    
                    for (int next : nextNeighbors) {
                        if (isValidPath(newPath, next)) {
                            validNeighbors.push_back(next);
                        }
                    }
                    
                    if (validNeighbors.size() > 1) {
                        int threadsPerBranch = threadsAvailable / validNeighbors.size();
                        int extraThreads = threadsAvailable % validNeighbors.size();
                        
                        std::vector<std::thread> threads;
                        int threadIdx = 0;
                        
                        for (size_t j = 0; j < validNeighbors.size() && !solutionFound.load(); j++) {
                            int branchThreads = threadsPerBranch + (j < extraThreads ? 1 : 0);
                            
                            if (branchThreads > 0 && threadIdx < threadsAvailable - 1) {
                                threads.emplace_back([this, newPath, validNeighbors, j, branchThreads]() {
                                    searchParallel(newPath, validNeighbors, j, j + 1, branchThreads);
                                });
                                threadIdx++;
                            } else {
                                std::vector<int> explorePath = newPath;
                                explorePath.push_back(validNeighbors[j]);
                                searchSequential(explorePath);
                            }
                        }
                        
                        for (auto& thread : threads) {
                            if (thread.joinable()) {
                                thread.join();
                            }
                        }
                        
                        if (solutionFound.load()) {
                            return;
                        }
                    } else {
                        searchSequential(newPath);
                    }
                } else {
                    searchSequential(newPath);
                }
            }
        }
    }

public:
    HamiltonianCycleFinder(const Graph& g, int start = 0) 
        : graph(g), solutionFound(false), startVertex(start) {}

    std::vector<int> findSequential() {
        solutionFound = false;
        solution.clear();
        
        std::vector<int> path = {startVertex};
        searchSequential(path);
        
        return solution;
    }

    std::vector<int> findParallel(int numThreads) {
        solutionFound = false;
        solution.clear();
        
        if (numThreads <= 1) {
            return findSequential();
        }

        std::vector<int> path = {startVertex};
        const std::vector<int>& neighbors = graph.getNeighbors(startVertex);
        
        if (neighbors.empty()) {
            return solution;
        }

        int threadsPerBranch = numThreads / neighbors.size();
        int extraThreads = numThreads % neighbors.size();
        
        std::vector<std::thread> threads;
        
        for (size_t i = 0; i < neighbors.size() && !solutionFound.load(); i++) {
            int branchThreads = threadsPerBranch + (i < extraThreads ? 1 : 0);
            
            if (branchThreads > 0 && threads.size() < numThreads - 1) {
                threads.emplace_back([this, path, neighbors, i, branchThreads]() {
                    searchParallel(path, neighbors, i, i + 1, branchThreads);
                });
            } else {
                searchParallel(path, neighbors, i, i + 1, 1);
            }
        }
        
        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        
        return solution;
    }

    void reset() {
        solutionFound = false;
        solution.clear();
    }
};

#endif
