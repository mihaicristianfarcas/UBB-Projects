import copy
import pickle
from src.domain.domain import Books


class MemoryRepository:
    def __init__(self):
        self.books_list = [Books(1234, "Fiodor_Dostoevsky", "Crime_and_Punishment"),
                           Books(2345, "Franz_Kafka", "The_Trial"),
                           Books(3456, "Ernest_Hemingway", "The_Old_Man_and_the_Sea"),
                           Books(4567, "Fiodor_Dostoevsky", "The_Brothers_Karamazov"),
                           Books(5678, "Fiodor_Dostoevsky", "White_Nights"),
                           Books(6789, "William_Faulkner", "The_Hamlet"),
                           Books(7890, "Ernest_Becker", "The_Denial_of_Death"),
                           Books(8901, "David_Epstein", "Range"),
                           Books(9012, "Jordan_Peterson", "12_Rules_for_Life"),
                           Books(1023, "Ernest_Hemingway", "Fiesta")]
        self.history = []

    def add(self, new_book):
        """
        Adds the new book to the repository
        :param new_book: the newly user-introduced book
        :return: False if the book is already in the repository, True otherwise
        """
        for i in range(len(self.books_list)):
            if new_book.isbn == self.books_list[i].isbn:
                return False

        self.history.append(copy.deepcopy(self.books_list))
        self.books_list.append(new_book)
        return True

    def filter(self, word):

        self.history.append(copy.deepcopy(self.books_list))
        significant_word = 0
        i = 0
        while i < len(self.books_list):
            current_book_title = self.books_list[i].title.split(sep='_')
            if word == current_book_title[significant_word]:
                self.books_list.pop(i)
                i -= 1
            i += 1

    def undo(self):
        self.books_list = copy.deepcopy(self.history.pop())


class TextFileRepository(MemoryRepository):

    def __init__(self):
        self.file_name = "textfile.txt"
        super().__init__()
        self.load_file()

    def add(self, new_book):
        super().add(new_book=new_book)
        self.save_file()

    def filter(self, word):
        super().filter(word)
        self.save_file()

    def undo(self):
        super().undo()
        self.save_file()

    def load_file(self):
        """
        Loads the data from the text file in data list as Books objects
        :return: None
        """
        isbn_index = 0
        author_index = 1
        title_index = 2
        self.books_list = []
        file = open(self.file_name, "r")
        for line in file:
            line = line.split()
            book = Books(int(line[isbn_index]), line[author_index], line[title_index])
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


class BinaryFileRepository(MemoryRepository):
    def __init__(self):
        self.file_name = "binaryfile.txt"
        super().__init__()
        self.save_file()

    def add(self, new_book):
        super().add(new_book=new_book)
        self.save_file()

    def filter(self, word):
        super().filter(word)
        self.save_file()

    def undo(self):
        super().undo()
        self.save_file()

    def load_file(self):
        """
        Loads the data from the binary file in data list
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
