import copy
import random

from src.domain.client import Client


class ClientRepository:
    def __init__(self):
        self.clients_list = []
        self.clients_list = generate_clients()
        self.history = []

    def add(self, new_client):

        for i in range(len(self.clients_list)):
            if new_client == self.clients_list[i]:
                return
        self.history.append(copy.deepcopy(self.clients_list))
        self.clients_list.append(new_client)

    def remove(self, client_id):

        self.history.append(copy.deepcopy(self.clients_list))
        i = 0
        while i < len(self.clients_list):
            client = self.clients_list[i]
            if client_id == client.client_id:
                self.clients_list.pop(i)
                break
            i += 1

    def update(self, client_id, new_name):

        self.history.append(copy.deepcopy(self.clients_list))
        i = 0
        while i < len(self.clients_list):
            client = self.clients_list[i]
            if client_id == client.client_id:
                self.clients_list[i].name = new_name
                break
            i += 1


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
