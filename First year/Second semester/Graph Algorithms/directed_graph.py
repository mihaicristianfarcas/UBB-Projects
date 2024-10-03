import copy
import random
import heapq
from collections import deque


def create_random_graph(num_vertices, num_edges):
    graph = DirectedGraph(num_vertices, 0)
    i = 0
    while i < num_edges and i < num_vertices**2:
        source = random.randint(0, num_vertices - 1)
        destination = random.randint(0, num_vertices - 1)
        if not graph.edge_exists(source, destination):
            graph.add_edge(source, destination, i)
            i += 1

    return graph


class DirectedGraph:

    def __init__(self, num_vertices=0, num_edges=0):
        self.num_vertices = num_vertices
        self.num_edges = num_edges
        self.inbound_edges = {}
        self.outbound_edges = {}
        for i in range(self.num_vertices):
            self.inbound_edges[i] = []
            self.outbound_edges[i] = []

        self.edges = {}

    def add_vertex(self, vertex_id):
        """
        Adds a new vertex to the graph!
        :param vertex_id: the vertex's associated integer
        :return: None
        """

        if vertex_id not in self.inbound_edges.keys() and vertex_id not in self.outbound_edges.keys():
            self.inbound_edges[vertex_id] = []
            self.outbound_edges[vertex_id] = []
            self.num_vertices += 1

    def add_edge(self, source, destination, cost):
        """
        Adds a new edge to the graph!
        :param source: the source vertex
        :param destination: the destination vertex
        :param cost: the integer value associated to an edge (edge tuple in the vertices dict), unique for each edge
        :return: None
        """
        if source in self.outbound_edges.keys() and destination in self.inbound_edges.keys():
            edge = (source, destination)
            self.outbound_edges[source].append(edge)  # append edge (a,b) to a
            self.inbound_edges[destination].append(edge)  # append edge (a,b) to b
            self.edges[edge] = cost
            self.num_edges += 1

    def remove_edge(self, source, destination):
        """
        Removes an edge from the graph!
        :param source: the source vertex
        :param destination: the destination vertex
        :return: None
        """
        edge = (source, destination)

        if source in self.outbound_edges.keys():
            self.outbound_edges[source] = [e for e in self.outbound_edges[source] if e != edge]
        if destination in self.inbound_edges.keys():
            self.inbound_edges[destination] = [e for e in self.inbound_edges[source] if e != edge]

        if edge in self.edges:
            del self.edges[edge]
            self.num_edges -= 1

    def remove_vertex(self, vertex_id):
        """
        Removes a vertex from the graph!
        :param vertex_id: the integer value associated to a vertex
        :return: None
        """
        if vertex_id in self.inbound_edges.keys():
            del self.inbound_edges[vertex_id]
        if vertex_id in self.outbound_edges.keys():
            del self.outbound_edges[vertex_id]

        self.edges = {edge: cost for edge, cost in self.edges.items() if vertex_id not in edge}
        self.num_vertices = len(self.inbound_edges)
        self.num_edges = len(self.edges)

    def get_num_vertices(self):
        """
        :return: the current number of vertices of the graph
        """
        return self.num_vertices

    def get_num_edges(self):
        """
        :return: the current number of edges of the graph
        """
        return self.num_edges

    def vertices_iter(self):
        """
        :return: an iterable version of the vertices set
        """
        return iter(self.inbound_edges.keys())

    def outbound_edges_iter(self, vertex_id):
        """
        :param vertex_id: the vertex for which the outbound edges are requested
        :return: an iterable version of the outbound edges of the given vertex
        """
        if vertex_id in self.outbound_edges.keys():
            return iter(self.outbound_edges[vertex_id])
        else:
            return iter([])

    def inbound_edges_iter(self, vertex_id):
        """
        :param vertex_id: the vertex for which the inbound edges are requested
        :return: an iterable version of the inbound edges of the given vertex
        """
        if vertex_id in self.inbound_edges.keys():
            return iter(self.inbound_edges[vertex_id])
        else:
            return iter([])

    def edge_exists(self, source, destination):
        """
        Checks if the edge given by its source and destination exists
        :param source: the edge's source vertex
        :param destination: the edge's destination vertex
        :return: True if the edge exists, False otherwise
        """
        edges_list = self.outbound_edges[source]
        for i in range(len(edges_list)):
            if (source, destination) == edges_list[i]:
                return True

        return False

    def get_cost(self, edge):
        """
        :param edge: the edge specified by the user
        :return: the cost of the given edge
        """
        return self.edges.get(edge)

    def set_cost(self, edge, cost):
        """
        Sets the cost of a given edge
        :param edge: the edge specified by the user
        :param cost: the cost of the given edge
        :return: None
        """
        self.edges[edge] = cost

    def get_in_degree(self, vertex_id):
        """
        :param vertex_id: the vertex for which the 'in' degree is requested
        :return: the 'in' degree of the given vertex
        """
        return len(self.inbound_edges.get(vertex_id, []))

    def get_out_degree(self, vertex_id):
        """
        :param vertex_id: the vertex for which the 'out' degree is requested
        :return: the 'out' degree of the given vertex
        """
        return len(self.outbound_edges.get(vertex_id, []))

    def console_print(self):

        source = 0
        destination = 1

        print(self.num_vertices, self.num_edges)
        for current_edge in self.edges.keys():
            print(current_edge[source], current_edge[destination], self.edges.get(current_edge))

    def save_file(self, filename):
        """
        Saves the graph's number of vertices, number of edges, the edges and their respective cost to a specified file
        :param filename: the file in which the data is being saved
        :return: None
        """
        source = 0
        destination = 1

        with open(filename, 'w') as file:
            file.write(f"{self.num_vertices} {self.get_num_edges()}\n")
            for current_edge in self.edges.keys():
                file.write(f"{current_edge[source]} {current_edge[destination]} {self.edges.get(current_edge)}\n")

    def load_file(self, filename):
        """
        Loads the graphs data from the given file
        :param filename: the file from which the data is taken
        :return: None
        """
        with open(filename, 'r') as file:
            line = file.readline().split()
            num_edges = int(line[1])
            for _ in range(num_edges):
                line = file.readline().split()
                source, destination, cost = int(line[0]), int(line[1]), int(line[2])
                self.add_vertex(source)
                self.add_vertex(destination)
                self.add_edge(source, destination, cost)

    def copy(self):
        """
        :return: a copy of the graph
        """
        return copy.deepcopy(self)

    def breadth_first_search(self, start, end):
        """
        Finds the shortest path between two given vertices using breadth-first search
        :param start: the source vertex
        :param end: the destination vertex
        :return: the shortest path between them as vertices in a list, None if the path doesn't exist
        """
        visited = set()
        queue = deque()
        paths = {start: [start]}  # Store the paths from start

        # Enqueue the start vertex and mark it as visited
        queue.append(start)
        visited.add(start)

        destination = 1

        while queue:
            current_vertex = queue.popleft()

            # Visit neighbors
            for neighbor_edge in self.outbound_edges_iter(current_vertex):
                neighbor_vertex = neighbor_edge[destination]  # Get the destination vertex of the edge
                if neighbor_vertex not in visited:
                    queue.append(neighbor_vertex)
                    visited.add(neighbor_vertex)
                    # Update the path to the neighbor
                    paths[neighbor_vertex] = paths[current_vertex] + [neighbor_vertex]

                    # If we reached the end vertex, return the shortest path
                    if neighbor_vertex == end:
                        return paths[end]

        # If end is not reachable from start
        return None

    def floyd_warshall(self, start_vertex, end_vertex):
        """
        The Floyd-Warshall algorithm for computing the minimum cost and
        the path "matrices" between two vertices
        :return: The minimum cost and path
        """
        # Initializing the costs matrix and path matrix
        num_vertices = self.get_num_vertices()
        cost_matrix = [[float('inf')] * num_vertices for _ in range(num_vertices)]
        path_matrix = [[-1] * num_vertices for _ in range(num_vertices)]

        destination = 1

        # Setting the costs for existing edges and updating path "matrices"
        for vertex in self.vertices_iter():
            cost_matrix[vertex][vertex] = 0
            for edge in self.outbound_edges_iter(vertex):
                neighbor_vertex = edge[destination]
                cost = self.get_cost((vertex, neighbor_vertex))
                cost_matrix[vertex][neighbor_vertex] = cost
                path_matrix[vertex][neighbor_vertex] = vertex

        # Updating costs and path "matrices" using Floyd-Warshall algorithm
        for k in range(num_vertices):
            for i in range(num_vertices):
                for j in range(num_vertices):
                    if cost_matrix[i][j] > cost_matrix[i][k] + cost_matrix[k][j]:
                        cost_matrix[i][j] = cost_matrix[i][k] + cost_matrix[k][j]
                        path_matrix[i][j] = path_matrix[k][j]

        cost = cost_matrix[start_vertex][end_vertex]
        path = []
        if path_matrix[start_vertex][end_vertex] == -1:
            return None  # No path exists between start_vertex and end_vertex

        # Reconstruct the path
        path.append(end_vertex)
        while start_vertex != end_vertex:
            end_vertex = path_matrix[start_vertex][end_vertex]
            path.append(end_vertex)
        path.reverse()

        return cost, path

    def prim_mst(self, start_vertex):
        """
        Finds the minimal spanning tree of the undirected version of the graph using Prim's algorithm
        :param start_vertex: the source vertex from where the search starts
        :return: the tree and the total cost
        """
        num_vertices = self.get_num_vertices()
        visited = set()
        mst_edges = []
        total_cost = 0

        # Priority queue: (cost, current_vertex, previous_vertex)
        # previous_vertex is used to store the edge in the MST
        pq = [(0, start_vertex, None)]
        heapq.heapify(pq)

        while len(mst_edges) < num_vertices - 1:
            cost, current_vertex, previous_vertex = heapq.heappop(pq)

            if current_vertex in visited:
                continue

            visited.add(current_vertex)
            if previous_vertex is not None:
                mst_edges.append((previous_vertex, current_vertex))
                total_cost += cost

            for edge in self.outbound_edges_iter(current_vertex):
                neighbor_vertex = edge[1]
                edge_cost = self.get_cost(edge)
                if neighbor_vertex not in visited:
                    heapq.heappush(pq, (edge_cost, neighbor_vertex, current_vertex))

            for edge in self.inbound_edges_iter(current_vertex):
                neighbor_vertex = edge[0]
                edge_cost = self.get_cost(edge)
                if neighbor_vertex not in visited:
                    heapq.heappush(pq, (edge_cost, neighbor_vertex, current_vertex))

        return mst_edges, total_cost

    def find_hamiltonian_cycle(self):
        """
        Finds a Hamiltonian cycle in the undirected version of the graph, if it exists.
        :return: A list representing the Hamiltonian cycle, or None if no cycle exists.
        """

        def is_valid_vertex(v, pos, path):
            # Check if the vertex has already been included
            if v in path:
                return False

            # Check if current vertex and last vertex in path are adjacent
            if (path[pos - 1], v) not in self.edges and (v, path[pos - 1]) not in self.edges:
                return False

            return True

        def hamiltonian_cycle_util(path, pos):
            # If all vertices are included in the path
            if pos == self.num_vertices:
                # And if there is an edge from the last included vertex to the first vertex
                if (path[pos - 1], path[0]) in self.edges or (path[0], path[pos - 1]) in self.edges:
                    return True
                else:
                    return False

            # Try different vertices as the next candidate in Hamiltonian cycle
            for v in range(self.num_vertices):
                if is_valid_vertex(v, pos, path):
                    path[pos] = v

                    # Recur to construct the rest of the path
                    if hamiltonian_cycle_util(path, pos + 1):
                        return True

                    # If adding vertex v doesn't lead to a solution, remove it
                    path[pos] = -1

            return False

        # Initialize path, all vertices are marked as not visited
        path = [-1] * self.num_vertices
        # Start from the first vertex
        path[0] = 0

        if not hamiltonian_cycle_util(path, 1):
            return None

        return path
