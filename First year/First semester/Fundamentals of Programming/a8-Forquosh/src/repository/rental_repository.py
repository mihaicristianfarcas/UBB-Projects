import datetime
import random

from src.domain.rental import Rental
from src.repository.book_repository import generate_books
from src.repository.client_repository import generate_clients


class RentalRepository:
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
