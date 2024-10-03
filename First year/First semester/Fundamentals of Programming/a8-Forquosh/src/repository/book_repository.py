import copy
import random

from src.domain.book import Book


class BookRepository:
    def __init__(self):
        self.books_list = []
        self.books_list = generate_books()
        self.history = []

    def add(self, new_book):

        self.history.append(copy.deepcopy(self.books_list))
        self.books_list.append(new_book)

    def remove(self, book_isbn):

        self.history.append(copy.deepcopy(self.books_list))
        i = 0
        while i < len(self.books_list):
            book = self.books_list[i]
            if book_isbn == book.isbn:
                self.books_list.pop(i)
                break
            i += 1

    def update(self, isbn, user_command, new_data):

        self.history.append(copy.deepcopy(self.books_list))
        author_update_command = 'A'
        title_update_command = 'B'

        i = 0
        while i < len(self.books_list):
            book = self.books_list[i]
            if isbn == book.isbn:
                if user_command == author_update_command:
                    self.books_list[i].author = new_data
                elif user_command == title_update_command:
                    self.books_list[i].title = new_data
                break


def generate_books():
    authors = ["Mihai_Eminescu", "Ioan_Slavici", "Fiodor_Dostoevsky", "Franz_Kafka", "Ernest_Hemingway",
               "Hermann_Hesse", "William_Faulkner", "Ernest_Becker", "David_Epstein", "Jordan_Peterson",
               "Lucian_Blaga", "Mircea_Eliade", "Friedrich_Nietzsche", "Immanuel_Kant", "Lev_Tolstoy"]

    titles = ["Crime_and_Punishment", "The_Trial", "Critique_of_Pure_Reason", "Thus_spoke_Zarathustra",
              "12_Rules_for_Life", "The_Brothers_Karamazov", "The_Metamorphosis", "Hronicul_si_Cantecul_Varstelor",
              "Sacrul_si_Profanul", "Siddhartha", "The_Denial_of_Death", "The_Hamlet", "Range", "Poezii"]

    isbn = 1000
    books_list = []
    for index in range(20):
        author = random.choice(authors)
        title = random.choice(titles)
        books_list.append(Book(isbn + index, author, title))

    return books_list
