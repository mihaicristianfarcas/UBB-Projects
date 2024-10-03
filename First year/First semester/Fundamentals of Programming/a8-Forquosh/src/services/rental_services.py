import datetime
from src.domain.rental import Rental
from src.repository.book_repository import BookRepository
from src.repository.client_repository import ClientRepository
from src.repository.rental_repository import RentalRepository


class RentalServices:

    def __init__(self):
        self.rental_repository = RentalRepository()
        self.book_repository = BookRepository()
        self.client_repository = ClientRepository()

    def display_most_rented_books(self):

        number_of_rentals = {}
        for rental in self.rental_repository.rentals_list:
            if rental.book.title not in number_of_rentals:
                number_of_rentals[rental.book.title] = 1
            else:
                number_of_rentals[rental.book.title] += 1

        number_of_rentals = sorted(number_of_rentals.items(), key=lambda x: x[1], reverse=True)
        return number_of_rentals

    def display_most_active_clients(self):

        client_and_rental_days = {}
        for rental in range(len(self.rental_repository.rentals_list)):
            rental_client = self.rental_repository.rentals_list[rental].client
            rental_client_name = rental_client.name
            client_and_rental_days[rental_client_name] = 0

        for rental in range(len(self.rental_repository.rentals_list)):
            rental_client = self.rental_repository.rentals_list[rental].client
            rental_client_name = rental_client.name
            client_and_rental_days[rental_client_name] += len(self.rental_repository.rentals_list[rental])

        client_and_rental_days = sorted(client_and_rental_days.items(), key=lambda x: x[1], reverse=True)

        return client_and_rental_days

    def display_most_rented_author(self):

        number_of_rentals = {}
        for rental in self.rental_repository.rentals_list:
            if rental.book.author not in number_of_rentals:
                number_of_rentals[rental.book.author] = 1
            else:
                number_of_rentals[rental.book.author] += 1

        number_of_rentals = sorted(number_of_rentals.items(), key=lambda x: x[1], reverse=True)
        return number_of_rentals

    def return_book(self, rental_id):
        try:
            rental_id = int(rental_id)
            if self.rental_repository.remove_rental(rental_id):
                return True
        except ValueError:
            return False

    def rent_book(self, client_id, isbn, min_rental_number=1000):
        try:
            isbn = int(isbn)
            client_id = int(client_id)
            new_rental_id = len(self.rental_repository.rentals_list) + min_rental_number
            rented_date = datetime.date.today()
            returned_date = rented_date + datetime.timedelta(days=14)
            if self.get_client(client_id) is False or self.get_book(isbn) is False:
                return 0
            client = self.get_client(client_id)
            book = self.get_book(isbn)
            new_rental = Rental(new_rental_id, book, client, rented_date, returned_date)
            self.rental_repository.add_rental(new_rental)
            return new_rental_id
        except RentalException:
            return 0

    def get_client(self, client_id):
        for i in range(len(self.client_repository.clients_list)):
            if client_id == self.client_repository.clients_list[i].client_id:
                return self.client_repository.clients_list[i]
        return False

    def get_book(self, isbn):
        for i in range(len(self.book_repository.books_list)):
            if isbn == self.book_repository.books_list[i].isbn:
                return self.book_repository.books_list[i]
        return False


class RentalException(Exception):
    def __init__(self, message):
        self._message = message

    def __str__(self):
        return str(self._message)
