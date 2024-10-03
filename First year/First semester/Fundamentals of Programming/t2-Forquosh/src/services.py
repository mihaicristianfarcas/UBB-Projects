from src.repository import Repository


class Services:

    def __init__(self):
        self.repository = Repository()

    def display_buses_on_route(self, route):
        bus_list = []
        for i in range(len(self.repository.bus_list)):
            if self.repository.bus_list[i].route_code == route:
                bus_list.append(self.repository.bus_list[i])

        return bus_list

    def compute_distance(self, bus_id):
        """
        Calculates and returns the distance traveled by the given bus on its route
        :param bus_id: the ID of the bus for which we want to find the traveled distance
        :return: the distance as an integer in kilometer
        """
        i = 0
        while True:
            if bus_id == self.repository.bus_list[i].bus_id:
                break
            i += 1

        j = 0
        while True:
            if self.repository.bus_list[i].route_code == self.repository.routes_list[j].route_code:
                break
            j += 1

        distance = int(self.repository.bus_list[i].times_on_route) * int(self.repository.routes_list[j].length_km)

        return self.repository.bus_list[i], distance

    def display_all_routes(self):
        pass

