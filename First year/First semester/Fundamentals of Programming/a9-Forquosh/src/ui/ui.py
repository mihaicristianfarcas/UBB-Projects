
from src.services.book_services import BookServices
from src.services.client_services import ClientServices
from src.services.rental_services import RentalServices

manage_command = add_command = '1'
rent_return_command = remove_command = '2'
search_command = update_command = '3'
create_statistics_command = display_command = '4'
book_related_command = rent_command = 'A'
client_related_command = return_command = 'B'
author_related_command = 'C'


class UI:

    def __init__(self):
        self.book_services = BookServices()
        self.client_services = ClientServices()
        self.rental_services = RentalServices()

    @staticmethod
    def print_menu():
        """
        Prints the initial menu options
        :return: None
        """
        print("Hello! Please choose an option:")
        print(" 1. Manage clients/books!")
        print(" 2. Rent/return a book!")
        print(" 3. Search for clients/books!")
        print(" 4. Create statistics!\n")

    @staticmethod
    def print_secondary_menu(option):
        """
        Prints the additional options' menu options
        :param option: the user initial menu's command
        :return: None
        """

        if option == manage_command:
            print(" A. Manage books!")
            print(" B. Manage clients!\n")

        if option == rent_return_command:
            print(" A. Rent a book!")
            print(" B. Return a book!\n")

        if option == search_command:
            print(" A. Search for books!")
            print(" B. Search for clients!\n")

        if option == create_statistics_command:
            print(" A. Show me the most rented books!")
            print(" B. Show me the most active clients!")
            print(" C. Show me the most rented authors!\n")

    @staticmethod
    def print_book_functionalities_specific_menu():
        """
        Prints the books specific menu options
        :return: None
        """
        print(" 1. Add a book!")
        print(" 2. Remove a book!")
        print(" 3. Update a book's data!")
        print(" 4. List all books!\n")

    @staticmethod
    def print_client_functionalities_specific_menu():
        """
        Prints the clients specific menu options
        :return: None
        """
        print(" 1. Add a client!")
        print(" 2. Remove a client!")
        print(" 3. Update a client's data!")
        print(" 4. List all clients!\n")

    @staticmethod
    def get_input():
        """
        Gets the user input
        :return: option (None if incorrect input)
        """
        try:
            option = input("Please choose which command you want to execute: ")
            assert option in [manage_command, rent_return_command, search_command, create_statistics_command]
            return option
        except AssertionError:
            print("Incorrect input! Please try again...\n")
            return None

    @staticmethod
    def get_secondary_menu_input():
        """
        Gets the user input
        :return: option (None if incorrect input)
        """
        try:
            option = input("Please choose which command you want to execute: ")
            assert option in [book_related_command, rent_command, return_command,
                              client_related_command, author_related_command]
            return option
        except AssertionError:
            print("Incorrect input! Please try again...\n")
            return None

    def process_command(self, user_initial_menu_command):
        """
        Processes the initial command and calls the corresponding service classes' methods
        :param user_initial_menu_command: the user-given initial command
        :return: None
        """

        self.print_secondary_menu(user_initial_menu_command)
        user_secondary_menu_command = self.get_secondary_menu_input()

        if user_initial_menu_command == manage_command:

            if user_secondary_menu_command == book_related_command:
                self.print_book_functionalities_specific_menu()
                user_specific_menu_command = self.get_input()

                if user_specific_menu_command == add_command:
                    isbn = input("ISBN: ")
                    author = input("Author: ")
                    title = input("Title: ")
                    if self.book_services.add_book(isbn, author, title):
                        print("Book added!\n")
                    else:
                        print("Incorrect ISBN or book already in repository! Please try again...\n")

                if user_specific_menu_command == remove_command:
                    isbn = input("Please give the ISBN of the book you want to remove: ")
                    if self.book_services.remove_book(isbn):
                        print("Book removed!\n")
                    else:
                        print("Incorrect ISBN or book not in repository! Please try again...\n")

                if user_specific_menu_command == update_command:

                    author_update_command = 'A'
                    title_update_command = 'B'

                    isbn = input("Please give the ISBN of the book you want to update: ")
                    print("Which attribute do you want to change?")
                    print(" A. Author\n", "B. Title\n")
                    user_command = input("Please choose which command you want to execute: ")
                    if user_command in [author_update_command, title_update_command]:
                        if user_command == author_update_command:
                            new_data = input("Please give the new author name: ")
                        else:
                            new_data = input("Please give the new title: ")

                        if self.book_services.update_book(isbn, user_command, new_data):
                            print("Book updated!\n")
                        else:
                            print("Incorrect ISBN/data or book not in repository! Please try again...\n ")
                    else:
                        print("Incorrect command! Please try again...\n")

                if user_specific_menu_command == display_command:
                    books_list = self.book_services.get_books()
                    for i in range(len(books_list)):
                        print("Book no.", i+1)
                        print(books_list[i])

            if user_secondary_menu_command == client_related_command:
                self.print_client_functionalities_specific_menu()
                user_specific_menu_command = self.get_input()

                if user_specific_menu_command == add_command:
                    client_id = input("ID: ")
                    name = input("Name: ")
                    if self.client_services.add_client(client_id, name):
                        print("Client added!\n")
                    else:
                        print("Incorrect ID or client already in repository! Please try again...\n")

                if user_specific_menu_command == remove_command:
                    client_id = input("Please give the ID of the client you want to remove: ")
                    if self.client_services.remove_client(client_id):
                        print("Client removed!\n")
                    else:
                        print("Incorrect ID or client not in repository! Please try again...\n")

                if user_specific_menu_command == update_command:
                    client_id = input("Please give the ID of the client you want to update: ")
                    new_name = input("Please give the new client's name: ")
                    if self.client_services.update_client(client_id, new_name):
                        print("Client updated!\n")
                    else:
                        print("Incorrect ID/name or client not in repository! Please try again...")

                if user_specific_menu_command == display_command:
                    clients_list = self.client_services.get_clients()
                    for i in range(len(clients_list)):
                        print("Client no.", i+1)
                        print(clients_list[i])

        if user_initial_menu_command == rent_return_command:
            print("Current rentals:\n")
            for i in range(len(self.rental_services.rental_repository.rentals_list)):
                print(self.rental_services.rental_repository.rentals_list[i], "\n")

            if user_secondary_menu_command == rent_command:
                client_id = input("Please give the renting client's ID: ")
                isbn = input("Please give the rented book's ISBN: ")
                rental_id = self.rental_services.rent_book(client_id, isbn)
                if rental_id != 0:
                    print(f"Book rented! Rental ID = {rental_id}\nAvailable for 2 weeks starting now.\n")
                else:
                    print("Book could not be rented. Please try again...\n")

            if user_secondary_menu_command == return_command:
                rental_id = input("Please give the rental ID: ")
                if self.rental_services.return_book(rental_id):
                    print("Book returned!\n")
                else:
                    print("Book could not be returned. Please try again...\n")

        if user_initial_menu_command == search_command:
            if user_secondary_menu_command == book_related_command:
                book_attribute = input("Please give any attribute of the book you want to find: ")
                self.book_services.search_book(book_attribute)

            if user_secondary_menu_command == client_related_command:
                client_attribute = input("Please give any attribute of the client you want to find: ")
                self.client_services.search_client(client_attribute)

        if user_initial_menu_command == create_statistics_command:
            if user_secondary_menu_command == book_related_command:
                most_rented_books = self.rental_services.display_most_rented_books()
                print("Most rented books:\n")
                for book in range(len(most_rented_books)):
                    print(f"{most_rented_books[book][0]} -> Rented {most_rented_books[book][1]} time/s!")
                print("\n")
            if user_secondary_menu_command == client_related_command:
                most_active_clients = self.rental_services.display_most_active_clients()
                print("Most rented books:\n")
                for client in range(len(most_active_clients)):
                    print(f"{most_active_clients[client][0]} -> Rented {most_active_clients[client][1]} days!")
                print("\n")
            if user_secondary_menu_command == author_related_command:
                most_rented_authors = self.rental_services.display_most_rented_author()
                print("Most rented authors:\n")
                for author in range(len(most_rented_authors)):
                    print(f"{most_rented_authors[author][0]} -> Rented {most_rented_authors[author][1]} time/s!")
                print("\n")

    def start(self):
        """
        Starts the program
        :return: None
        """
        while True:
            self.print_menu()
            user_initial_menu_command = self.get_input()
            if user_initial_menu_command is not None:
                self.process_command(user_initial_menu_command)


if __name__ == "__main__":
    user_interface = UI()
    user_interface.start()
