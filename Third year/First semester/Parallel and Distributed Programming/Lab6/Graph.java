import java.util.*;

public class Graph {
    private final int numVertices;
    private final List<List<Integer>> adjacencyList;

    public Graph(int vertices) {
        this.numVertices = vertices;
        this.adjacencyList = new ArrayList<>(vertices);
        for (int i = 0; i < vertices; i++) {
            adjacencyList.add(new ArrayList<>());
        }
    }

    public void addEdge(int from, int to) {
        adjacencyList.get(from).add(to);
    }

    public int getNumVertices() {
        return numVertices;
    }

    public List<Integer> getNeighbors(int vertex) {
        return adjacencyList.get(vertex);
    }

    public void print() {
        System.out.println("Graph with " + numVertices + " vertices:");
        for (int i = 0; i < numVertices; i++) {
            System.out.print(i + " -> ");
            for (int neighbor : adjacencyList.get(i)) {
                System.out.print(neighbor + " ");
            }
            System.out.println();
        }
    }

    public static Graph createSampleGraph1() {
        Graph g = new Graph(5);
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

    public static Graph createSampleGraph2() {
        Graph g = new Graph(6);
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

    public static Graph createNoHamiltonianGraph() {
        Graph g = new Graph(4);
        g.addEdge(0, 1);
        g.addEdge(1, 2);
        g.addEdge(2, 3);
        return g;
    }

    public static Graph createLargerGraph(int size) {
        Graph g = new Graph(size);
        
        for (int i = 0; i < size; i++) {
            g.addEdge(i, (i + 1) % size);
        }
        
        for (int i = 0; i < size; i++) {
            if (i + 2 < size) {
                g.addEdge(i, i + 2);
            }
            if (i > 1) {
                g.addEdge(i, i - 2);
            }
        }
        
        return g;
    }
}
