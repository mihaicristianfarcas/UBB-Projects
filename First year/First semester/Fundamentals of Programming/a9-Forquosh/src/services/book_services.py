from src.domain.book import Book
from src.repository.book_repository import BookMemoryRepository


class BookServices:
    def __init__(self):
        self.book_repository = BookMemoryRepository()

    def add_book(self, isbn, author, title):
        """
        Adds a book to the list as a Book object
        :return: None
        """
        try:
            isbn = int(isbn)
            assert 1000 <= isbn <= 9999
            book = Book(isbn, author, title)
            if not self.is_isbn_in_list(isbn):
                self.book_repository.add(book)
                return True
        except (ValueError, AssertionError):
            return False

    def remove_book(self, isbn) -> bool:
        """
        Removes the book from the list using its ISBN
        :return: True if the book has been successfully removed, False otherwise
        """
        try:
            isbn = int(isbn)
            assert 1000 <= isbn <= 9999
            if self.is_isbn_in_list(isbn):
                self.book_repository.remove(isbn)
                return True
            else:
                return False
        except (ValueError, AssertionError):
            return False

    def update_book(self, isbn, user_command, new_data):
        """
        Updates a book's parameter with new user-given data
        :param isbn: updated book's isbn
        :param user_command: the command which tells which parameter the user wants to change
        :param new_data: parameter's new data
        :return: True if the book's data has been successfully updated, False otherwise
        """

        author_update_command = 'A'
        title_update_command = 'B'

        try:
            isbn = int(isbn)
            new_data = str(new_data)
            assert 1000 <= isbn <= 9999
            if self.is_isbn_in_list(isbn):
                if user_command == author_update_command:
                    self.book_repository.update(isbn, author_update_command, new_data)
                else:
                    self.book_repository.update(isbn, title_update_command, new_data)
                return True
            else:
                return False
        except (ValueError, AssertionError):
            return False

    def display_books(self):
        """
        Prints the books nicely to the console
        :return: None
        """
        for i in range(len(self.book_repository.books_list)):
            print("Book no.", i+1)
            print(self.book_repository.books_list[i])

    def search_book(self, book_attribute):
        """
        Searches for a book in the repository by any given attribute
        :param book_attribute: the user-given attribute by which the searching is done
        :return: None
        """
        try:
            book_attribute = int(book_attribute)
            assert 1000 <= book_attribute <= 9999
            isbn_is_in_list = False

            i = 0
            while i < len(self.book_repository.books_list):
                if book_attribute == self.book_repository.books_list[i].isbn:
                    isbn_is_in_list = True
                    break
                i += 1

            if isbn_is_in_list is True:
                print("Found a match!")
                print("Book no.", i + 1)
                print(self.book_repository.books_list[i])
            else:
                print("ISBN not in list! Please try again...\n")

        except (ValueError, AssertionError):
            i = 0
            match_found = False

            while i < len(self.book_repository.books_list):
                book = self.book_repository.books_list[i]
                if book_attribute == book.author or book_attribute == book.title:
                    match_found = True
                    print("Found a match!")
                    print("Book no.", i + 1)
                    print(self.book_repository.books_list[i])
                i += 1

            if match_found is False:
                print("Incorrect input or no books match your search!\n")

    def is_isbn_in_list(self, book_isbn):
        """
        Checks if the ISBN introduced by the user is in the book list
        :param book_isbn: The user-given ISBN
        :return: True/False
        """

        for i in range(len(self.book_repository.books_list)):
            if book_isbn == self.book_repository.books_list[i].isbn:
                return True
        return False

    def get_books(self):
        return self.book_repository.get_books_list()
