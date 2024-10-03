import copy
import pickle
import random

from src.domain.client import Client


class ClientMemoryRepository:
    def __init__(self):
        self.clients_list = []
        self.clients_list = generate_clients()

    def add(self, new_client):

        for i in range(len(self.clients_list)):
            if new_client == self.clients_list[i]:
                return
        self.history.append(copy.deepcopy(self.clients_list))
        self.clients_list.append(new_client)

    def remove(self, client_id):

        i = 0
        while i < len(self.clients_list):
            client = self.clients_list[i]
            if client_id == client.client_id:
                self.clients_list.pop(i)
                break
            i += 1

    def update(self, client_id, new_name):

        i = 0
        while i < len(self.clients_list):
            client = self.clients_list[i]
            if client_id == client.client_id:
                self.clients_list[i].name = new_name
                break
            i += 1

    def get_clients_list(self):
        return self.clients_list


class ClientTextFileRepository(ClientMemoryRepository):

    def __init__(self):
        self.file_name = "client_textfile.txt"
        super().__init__()
        self.save_file()

    def add(self, new_client):
        super().add(new_client=new_client)
        self.save_file()

    def remove(self, client_id):
        super().remove(client_id=client_id)
        self.save_file()

    def update(self, client_id, new_name):
        super().update(client_id=client_id, new_name=new_name)
        self.save_file()

    def load_file(self):
        """
        Loads the data from the text file in data list as Client objects
        :return: None
        """
        client_id_index = 0
        name_index = 1
        self.clients_list = []
        file = open(self.file_name, "r")
        for line in file:
            line = line.split()
            client = Client(int(line[client_id_index]), line[name_index])
            self.clients_list.append(client)
        file.close()

    def save_file(self):
        """
        Saves the data in the text file
        :return: None
        """
        with open(self.file_name, "w") as file:
            for i in range(len(self.clients_list)):
                print(self.clients_list[i].client_id, self.clients_list[i].name, file=file)
        file.close()


class ClientBinaryFileRepository(ClientMemoryRepository):
    def __init__(self):
        self.file_name = "client_binaryfile.txt"
        super().__init__()
        self.save_file()

    def add(self, new_client):
        super().add(new_client=new_client)
        self.save_file()

    def remove(self, client_id):
        super().remove(client_id=client_id)
        self.save_file()

    def update(self, client_id, new_name):
        super().update(client_id=client_id, new_name=new_name)
        self.save_file()

    def load_file(self):
        """
        Loads the data from the binary file in data list
        :return: None
        """
        file = open(self.file_name, "rb")
        self.clients_list = pickle.load(file)
        file.close()

    def save_file(self):
        """
        Saves data in the binary file
        :return: None
        """
        file = open(self.file_name, "wb")
        pickle.dump(self.clients_list, file)
        file.close()


def generate_clients():
    first_names = ["Ana", "Maria", "Adrian", "Alex", "Mihai", "Diana", "Daria", "Mirela", "Abel", "John", "Ioana"]
    last_names = ["Popescu", "Marinescu", "Farcas", "Lebada", "Fonai", "Caldarar", "Crisan", "Ganea", "Rosiu", "Cristea"]

    clients = []
    client_id = 1000
    for index in range(20):
        first_name = random.choice(first_names)
        last_name = random.choice(last_names)
        clients.append(Client(client_id + index, first_name + "_" + last_name))

    return clients
