from directed_graph import DirectedGraph, create_random_graph


class UI:

    def __init__(self):
        self.graph = DirectedGraph()
        self.graph_copy = DirectedGraph()

    @staticmethod
    def print_menu():
        print("\nHello! Please choose an action:")
        print("1. Get the number of vertices.")
        print("2. Iterate the vertices.")
        print("3. Find edge between given source and destination vertices.")
        print("4. Get the in and out degree of a vertex.")
        print("5. Iterate the outbound edges of a vertex.")
        print("6. Iterate the inbound edges of a vertex.")
        print("7. Modify the cost of an edge.")
        print("8. Add an edge.")
        print("9. Remove an edge.")
        print("10. Add a vertex.")
        print("11. Remove a vertex.")
        print("12. Make a copy of the graph.")
        print("13. Retrieve the copy of the graph.")
        print("14. Read the graph from a text file.")
        print("15. Write the graph to a text file.")
        print("16. Generate a random graph.")
        print("17. Print the graph to the console.")
        print("18. Find the shortest path between two given vertices using breadth-first search.")
        print("19. Find the minimum cost and shortest path between two given vertices using the Floyd-Warshall algorithm.")
        print("20. Find a minimal spanning tree using Prim's algorithm.")
        print("21. Find a Hamiltonian cycle in the undirected graph, if it exists.")
        print("0. Exit program.")

    def menu(self):

        while True:

            self.print_menu()
            user_option = int(input("> "))
            if user_option == 0:
                print("Bye!")
                break
            if user_option == 1:
                print(self.graph.num_vertices)

            if user_option == 2:
                for i in self.graph.vertices_iter():
                    print(i)

            if user_option == 3:
                source = int(input("Please give the source vertex: "))
                destination = int(input("Please give the destination vertex: "))
                print(self.graph.edge_exists(source, destination))

            if user_option == 4:
                vertex = int(input("Please give the vertex: "))
                print("In :", self.graph.get_in_degree(vertex))
                print("Out: ", self.graph.get_out_degree(vertex))

            if user_option == 5:
                vertex = int(input("Please give the vertex: "))
                for i in self.graph.outbound_edges_iter(vertex):
                    print(i)

            if user_option == 6:
                vertex = int(input("Please give the vertex: "))
                for i in self.graph.inbound_edges_iter(vertex):
                    print(i)

            if user_option == 7:
                source = int(input("Please give the source vertex: "))
                destination = int(input("Please give the destination vertex: "))
                new_cost = int(input("Please give the new cost of the edge: "))
                self.graph.set_cost((source, destination), new_cost)

            if user_option == 8:
                source = int(input("Please give the source vertex: "))
                destination = int(input("Please give the destination vertex: "))
                cost = int(input("Please give the cost of the edge: "))
                if self.graph.add_edge(source, destination, cost):
                    print("Edge added!\n")
                else:
                    print("Edge could not be added. Please try again...\n")

            if user_option == 9:
                source = int(input("Please give the source vertex: "))
                destination = int(input("Please give the destination vertex: "))
                self.graph.remove_edge(source, destination)

            if user_option == 10:
                vertex = int(input("Please give the vertex: "))
                self.graph.add_vertex(vertex)

            if user_option == 11:
                vertex = int(input("Please give the vertex: "))
                self.graph.remove_vertex(vertex)

            if user_option == 12:
                self.graph_copy = self.graph.copy()

            if user_option == 13:
                self.graph = self.graph_copy.copy()

            if user_option == 14:
                self.graph.load_file("random_graph1.txt")

            if user_option == 15:
                self.graph.save_file("random_graph.txt")

            if user_option == 16:
                nr_vertices = int(input("Please give the number of vertices: "))
                nr_edges = int(input("Please give the number of edges: "))
                self.graph = create_random_graph(nr_vertices, nr_edges)

            if user_option == 17:
                self.graph.console_print()

            if user_option == 18:
                source = int(input("Please give the source vertex: "))
                destination = int(input("Please give the destination vertex: "))
                shortest_path = self.graph.breadth_first_search(source, destination)
                if shortest_path:
                    print("Path length:", len(shortest_path) - 1)
                    print("Shortest path:\n", shortest_path)
                else:
                    print("No path exists between the start and end vertices.")

            if user_option == 19:
                source = int(input("Please give the source vertex: "))
                destination = int(input("Please give the destination vertex: "))
                minimum_cost, shortest_path = self.graph.floyd_warshall(source, destination)
                print("Minimum cost: ", minimum_cost, "\nShortest path: ", shortest_path)

            if user_option == 20:
                source = int(input("Please give the source vertex: "))
                tree, total_cost = self.graph.prim_mst(source)
                print("Tree: ", tree, "\nTotal cost: ", total_cost)

            if user_option == 21:
                cycle = self.graph.find_hamiltonian_cycle()
                if cycle:
                    print("Cycle is:\n", cycle)
                else:
                    print("No cycle exists.")


if __name__ == '__main__':
    ui = UI()
    ui.menu()
