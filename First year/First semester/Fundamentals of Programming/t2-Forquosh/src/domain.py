class Bus:

    def __init__(self, bus_id, route_code, model, times_on_route):
        self._bus_id = bus_id
        self._route_code = route_code
        self._model = model
        self._times_on_route = times_on_route

    @property
    def route_code(self):
        return self._route_code

    @property
    def times_on_route(self):
        return self._times_on_route

    @property
    def bus_id(self):
        return self._bus_id

    def __str__(self):
        return f'Bus:\nID: {self._bus_id} ; Route code: {self._route_code} ; Model: {self._model} ; Times on route: {self._times_on_route}\n'

    @route_code.setter
    def route_code(self, value):
        self._route_code = value


class Route:

    def __init__(self, route_code, length_km):
        self._route_code = route_code
        self.length_km = length_km

    @property
    def route_code(self):
        return self._route_code

    def __str__(self):
        pass


