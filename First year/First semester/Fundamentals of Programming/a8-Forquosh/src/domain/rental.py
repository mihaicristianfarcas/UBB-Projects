import datetime as date
from src.domain.book import Book
from src.domain.client import Client


class Rental:
    def __init__(self, rental_id: int, book: Book, client: Client, rented_date: date, returned_date: date):
        self._rental_id = rental_id
        self._book = book
        self._client = client
        self._rented_date = rented_date
        self._returned_date = returned_date

    @property
    def rental_id(self):
        return self._rental_id

    @property
    def client(self):
        return self._client

    @property
    def book(self):
        return self._book

    @property
    def rented_date(self):
        return self._rented_date

    @property
    def returned_date(self):
        return self._returned_date

    def __eq__(self, other):
        if not isinstance(other, Rental):
            return False
        return self.rental_id == other.rental_id

    def __len__(self):
        return (self._returned_date - self._rented_date).days

    def __str__(self):
        return ("Rental ID -> " + str(self._rental_id) + "\nBook -> " + str(self._book) + "Client -> " +
                str(self._client) + "Period -> " + self._rented_date.strftime("%d-%m-%y") +
                " to " + self._returned_date.strftime("%d-%m-%y"))
