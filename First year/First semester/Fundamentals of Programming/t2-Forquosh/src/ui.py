from src.services import Services

display_buses = 1
compute_km = 2
display_routes = 3


class UI:

    def __init__(self):
        self.services = Services()

    def start(self):
        print("Please choose an option:\n")
        print("1. Display all buses travelling across a certain route")
        print("2. Compute how many kilometers a bus has traveled on a given route")
        print("3. Display bus routes in descending order of total mileage")

        while True:
            option = int(input("> "))

            if option == display_buses:
                route = int(input("Please specify the route: "))
                result = self.services.display_buses_on_route(route)
                for i in range(len(result)):
                    print(result[i])
            if option == compute_km:
                bus_id = int(input("Please give the bus ID: "))
                bus, distance = self.services.compute_distance(bus_id)
                print(bus)
                print("Distance:", distance)
            if option == display_routes:
                self.services.display_all_routes()


if __name__ == '__main__':
    ui = UI()
    ui.start()
