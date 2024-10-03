import pickle
import random

from src.domain.book import Book


class BookMemoryRepository:
    def __init__(self):
        self.books_list = []
        self.books_list = generate_books()

    def add(self, new_book):

        self.books_list.append(new_book)

    def remove(self, book_isbn):

        i = 0
        while i < len(self.books_list):
            book = self.books_list[i]
            if book_isbn == book.isbn:
                self.books_list.pop(i)
                break
            i += 1

    def update(self, isbn, user_command, new_data):

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

    def get_books_list(self):
        return self.books_list


class BookTextFileRepository(BookMemoryRepository):

    def __init__(self):
        self.file_name = "book_textfile.txt"
        super().__init__()
        self.save_file()

    def add(self, new_book):
        super().add(new_book=new_book)
        self.save_file()

    def remove(self, book_isbn):
        super().remove(book_isbn=book_isbn)
        self.save_file()

    def update(self, isbn, user_command, new_data):
        super().update(isbn=isbn, user_command=user_command, new_data=new_data)
        self.save_file()

    def load_file(self):
        """
        Loads the data from the text file in data list as Book objects
        :return: None
        """
        isbn_index = 0
        author_index = 1
        title_index = 2
        self.books_list = []
        file = open(self.file_name, "r")
        for line in file:
            line = line.split()
            book = Book(int(line[isbn_index]), line[author_index], line[title_index])
            self.books_list.append(book)
        file.close()

    def save_file(self):
        """
        Saves the data in the text file
        :return: None
        """
        with open(self.file_name, "w") as file:
            for i in range(len(self.books_list)):
                print(self.books_list[i].isbn, self.books_list[i].author, self.books_list[i].title, file=file)
        file.close()


class BookBinaryFileRepository(BookMemoryRepository):

    def __init__(self):
        self.file_name = "book_binaryfile.txt"
        super().__init__()
        self.save_file()

    def add(self, new_book):
        super().add(new_book=new_book)
        self.save_file()

    def remove(self, book_isbn):
        super().remove(book_isbn=book_isbn)
        self.save_file()

    def update(self, isbn, user_command, new_data):
        super().update(isbn=isbn, user_command=user_command, new_data=new_data)
        self.save_file()

    def load_file(self):
        """
        Loads the data from the text file in data list as a Book objects
        :return: None
        """
        file = open(self.file_name, "rb")
        self.books_list = pickle.load(file)
        file.close()

    def save_file(self):
        """
        Saves data in the binary file
        :return: None
        """
        file = open(self.file_name, "wb")
        pickle.dump(self.books_list, file)
        file.close()


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
