from src.domain.client import Client
from src.repository.client_repository import ClientMemoryRepository


class ClientServices:
    def __init__(self):
        self.client_repository = ClientMemoryRepository()

    def add_client(self, client_id, name):
        """
        Adds a client to the list as a Client object
        :return: True if the new client has been successfully added, False otherwise
        """
        try:
            client_id = int(client_id)
            assert 1000 <= client_id <= 9999
            client = Client(client_id, name)
            if not self.is_client_in_list(client_id):
                self.client_repository.add(client)
                return True
        except (ValueError, AssertionError):
            return False

    def remove_client(self, client_id):
        """
        Removes the given client
        :return: True if the client has been successfully removed, False otherwise
        """

        try:
            client_id = int(client_id)
            assert 1000 <= client_id <= 9999
            if self.is_client_in_list(client_id):
                self.client_repository.remove(client_id)
                return True
            else:
                return False
        except (ValueError, AssertionError):
            return False

    def update_client(self, client_id, new_name):
        """
        Updates the given client's data
        :param client_id: updated client's id
        :param new_name: the new name attributed to the given client
        :return: True if the client's name has been successfully updated, False otherwise
        """

        try:
            client_id = int(client_id)
            new_name = str(new_name)
            assert 1000 <= client_id <= 9999
            if self.is_client_in_list(client_id):
                self.client_repository.update(client_id, new_name)
                return True
            else:
                return False
        except (ValueError, AssertionError):
            return False

    def get_clients(self):
        """
        Prints the clients nicely to the console
        :return: None
        """
        return self.client_repository.get_clients_list()

    def search_client(self, client_attribute):
        """
        Searches for the required client in the client list by any given attribute
        :param client_attribute: the user-given attribute by which the searching is done
        :return: None
        """

        try:
            client_attribute = int(client_attribute)
            assert 1000 <= client_attribute <= 9999
            client_id_is_in_list = False

            i = 0
            while i < len(self.client_repository.clients_list):
                if client_attribute == self.client_repository.clients_list[i].client_id:
                    client_id_is_in_list = True
                    break
                i += 1

            if client_id_is_in_list is True:
                print("Found a match!")
                print("Client no.", i + 1)
                print(self.client_repository.clients_list[i])
            else:
                print("ID not in list! Please try again...\n")

        except (ValueError, AssertionError):
            i = 0
            match_found = False

            while i < len(self.client_repository.clients_list):
                client = self.client_repository.clients_list[i]
                if client_attribute == client.client_id or client_attribute == client.name:
                    match_found = True
                    print("Found a match!")
                    print("Client no.", i + 1)
                    print(self.client_repository.clients_list[i])
                i += 1

            if match_found is False:
                print("Incorrect input or no clients match your search!\n")

    def is_client_in_list(self, client_id):
        """
        Checks if the ID introduced by the user is in the client list
        :param client_id: The user-given ID
        :return: True/False
        """

        for i in range(len(self.client_repository.clients_list)):
            if client_id == self.client_repository.clients_list[i].client_id:
                return True
        return False
