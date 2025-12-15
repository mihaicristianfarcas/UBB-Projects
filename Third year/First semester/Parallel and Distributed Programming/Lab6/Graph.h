#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_set>
#include <algorithm>
#include <iostream>

class Graph {
private:
    int numVertices;
    std::vector<std::vector<int>> adjacencyList;

public:
    Graph(int vertices) : numVertices(vertices), adjacencyList(vertices) {}

    void addEdge(int from, int to) {
        adjacencyList[from].push_back(to);
    }

    int getNumVertices() const {
        return numVertices;
    }

    const std::vector<int>& getNeighbors(int vertex) const {
        return adjacencyList[vertex];
    }

    void print() const {
        std::cout << "Graph with " << numVertices << " vertices:\n";
        for (int i = 0; i < numVertices; i++) {
            std::cout << i << " -> ";
            for (int neighbor : adjacencyList[i]) {
                std::cout << neighbor << " ";
            }
            std::cout << "\n";
        }
    }

    static Graph createSampleGraph1() {
        Graph g(5);
        g.addEdge(0, 1);
        g.addEdge(1, 2);
        g.addEdge(2, 3);
        g.addEdge(3, 4);
        g.addEdge(4, 0);
        g.addEdge(0, 3);
        g.addEdge(1, 4);
        g.addEdge(2, 0);
        return g;
    }

    static Graph createSampleGraph2() {
        Graph g(6);
        g.addEdge(0, 1);
        g.addEdge(0, 5);
        g.addEdge(1, 2);
        g.addEdge(1, 4);
        g.addEdge(2, 3);
        g.addEdge(3, 4);
        g.addEdge(3, 5);
        g.addEdge(4, 0);
        g.addEdge(4, 5);
        g.addEdge(5, 2);
        return g;
    }

    static Graph createNoHamiltonianGraph() {
        Graph g(4);
        g.addEdge(0, 1);
        g.addEdge(1, 2);
        g.addEdge(2, 3);
        return g;
    }
};

#endif
