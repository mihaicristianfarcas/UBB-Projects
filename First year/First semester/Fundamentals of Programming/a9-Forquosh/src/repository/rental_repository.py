import datetime
import pickle
import random
from time import strptime

from src.domain.book import Book
from src.domain.client import Client
from src.domain.rental import Rental
from src.repository.book_repository import generate_books
from src.repository.client_repository import generate_clients


class RentalMemoryRepository:
    def __init__(self):
        self.rentals_list = []
        self.rentals_list = generate_rentals()

    def __str__(self):
        return str(self.rentals_list)

    def add_rental(self, new_rental):
        self.rentals_list.append(new_rental)

    def remove_rental(self, rental_id):
        for i in range(len(self.rentals_list)):
            if rental_id == self.rentals_list[i].rental_id:
                self.rentals_list.pop(i)
                return True
        return False


class RentalTextFileRepository(RentalMemoryRepository):

    def __init__(self):
        self.file_name = "rental_textfile.txt"
        super().__init__()
        self.save_file()

    def add_rental(self, new_rental):
        super().add_rental(new_rental=new_rental)
        self.load_file()

    def remove_rental(self, rental_id):
        super().remove_rental(rental_id=rental_id)

    def load_file(self):
        """
        Loads the data from the text file in data list as Rental objects
        :return: None
        """
        rental_id_index = 0
        book_isbn_index = 1
        book_author_index = 2
        book_title_index = 3
        client_id_index = 4
        client_name_index = 5
        rented_date_index = 6
        returned_date_index = 7
        self.rentals_list = []
        file = open(self.file_name, "r")
        for line in file:
            line = line.split()

            rented_date_split = line[rented_date_index].split("-")
            rented_date = datetime.date(int(rented_date_split[0]), int(rented_date_split[1]), int(rented_date_split[2]))

            returned_date_split = line[returned_date_index].split("-")
            returned_date = datetime.date(int(returned_date_split[0]), int(returned_date_split[1]), int(returned_date_split[2]))

            rental = Rental(int(line[rental_id_index]), Book(int(line[book_isbn_index]), line[book_author_index], line[book_title_index]),
                            Client(int(line[client_id_index]), line[client_name_index]), rented_date, returned_date)
            self.rentals_list.append(rental)
        file.close()

    def save_file(self):
        """
        Saves the data in the text file
        :return: None
        """
        with open(self.file_name, "w") as file:
            for i in range(len(self.rentals_list)):
                print(self.rentals_list[i].rental_id, self.rentals_list[i].book.isbn, self.rentals_list[i].book.author,
                      self.rentals_list[i].book.title, self.rentals_list[i].client.client_id, self.rentals_list[i].client.name,
                      self.rentals_list[i].rented_date, self.rentals_list[i].returned_date, file=file)
        file.close()


class RentalBinaryFileRepository(RentalMemoryRepository):

    def __init__(self):
        super().__init__()
        self.file_name = "rental_binaryfile.txt"
        self.save_file()

    def add_rental(self, new_rental):
        super().add_rental(new_rental=new_rental)
        self.save_file()

    def remove_rental(self, rental_id):
        super().remove_rental(rental_id=rental_id)

    def load_file(self):
        """
        Loads the data from the text file in data list as a Book objects
        :return: None
        """
        file = open(self.file_name, "rb")
        self.rentals_list = pickle.load(file)
        file.close()

    def save_file(self):
        """
        Saves data in the binary file
        :return: None
        """
        file = open(self.file_name, "wb")
        pickle.dump(self.rentals_list, file)
        file.close()


def generate_rentals():
    books_list = generate_books()
    clients_list = generate_clients()
    rentals_list = []
    rental_id = 1000

    for index in range(20):
        book = random.choice(books_list)
        client = random.choice(clients_list)

        rented_date = datetime.date.today() + datetime.timedelta(days=random.randint(-60, -10))
        returned_date = rented_date + datetime.timedelta(days=14)

        rentals_list.append(Rental(rental_id + index, book, client, rented_date, returned_date))

    return rentals_list
