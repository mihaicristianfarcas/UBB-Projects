from src.domain import Bus, Route


class Repository:

    def __init__(self):
        self.bus_filename = "buses.txt"
        self.routes_filename = "bus_routes.txt"
        self.bus_list = []
        self.routes_list = []
        self.load_file()

    def load_file(self):
        bus_file = open(self.bus_filename, 'r')
        for line in bus_file:
            line = line.split(", ")
            bus = Bus(int(line[0]), int(line[1]), line[2], int(line[3]))
            self.bus_list.append(bus)

        routes_file = open(self.routes_filename, 'r')
        for line in routes_file:
            line = line.split(", ")
            route = Route(int(line[0]), int(line[1]))
            self.routes_list.append(route)
