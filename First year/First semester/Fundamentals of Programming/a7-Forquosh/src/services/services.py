from src.domain.domain import Books
from src.repository.repository import MemoryRepository, TextFileRepository, BinaryFileRepository


class Services:
    def __init__(self):
        self.repository = TextFileRepository()
        # MemoryRepository()
        # TextFileRepository()
        # BinaryFileRepository()

    def add_book(self, isbn, author, title):
        """
        Adds a book to the list as a dictionary
        :return: True if book has been successfully added, False otherwise
        """
        book = Books(isbn, author, title)
        if book.isbn is not None:
            if self.repository.add(book):
                return True
        return False

    def display_books(self):
        """
        Prints the books nicely to the console
        :return: None
        """

        for i in range(len(self.repository.books_list)):
            print("Book no.", i+1)
            print(self.repository.books_list[i])

    def filter_books_list(self, word):
        """
        Filters the list by the user-given word such that the book titles containing the word get removed
        :return: None
        """
        self.repository.filter(word)

    def undo(self):
        """
        Undo the last operation
        :return: False if undo not possible, True otherwise
        """
        if len(self.repository.history) == 0:
            return False
        else:
            self.repository.undo()
            return True
