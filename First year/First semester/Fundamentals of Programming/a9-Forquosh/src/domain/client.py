class Client:
    def __init__(self, client_id, name):
        if not isinstance(client_id, int):
            raise TypeError()
        if not isinstance(name, str):
            raise TypeError()

        self._id = client_id
        self._name = name

    @property
    def client_id(self):
        return self._id

    @property
    def name(self):
        return self._name

    @name.setter
    def name(self, new_name):
        self._name = new_name

    def __str__(self):
        return f'ID: {self._id} ; Name: {self._name}\n'

    def __eq__(self, other):
        if not isinstance(other, Client):
            return False
        return self._id == other._id
