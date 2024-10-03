from src.services.services import Services

undo_command = '0'
add_command = '1'
display_command = '2'
filter_command = '3'


class UI:

    def __init__(self):
        self.service = Services()
        self.commands = {"1": Services.add_book,
                         "2": Services.display_books,
                         "3": Services.filter_books_list,
                         "0": Services.undo}
        self.option = 0

    @staticmethod
    def print_menu():
        """
        Prints the menu options
        :return: None
        """
        print("Hello! Please choose an option:")
        print("1. Add a book!")
        print("2. Display the list of books!")
        print("3. Filter the list (by a given word)!")
        print("0. Undo the last operation!\n")

    def get_input(self):
        """
        Gets the user input
        :return: None
        """
        try:
            self.option = input("Please choose which command you want to execute: ")
            assert self.option in [undo_command, add_command, display_command, filter_command]
            return self.option
        except AssertionError:
            print("Incorrect input! Please try again...\n")
            return None

    def process_command(self, user_command):

        if user_command == add_command:
            isbn = input("ISBN: ")
            author = input("Author: ")
            title = input("Title: ")
            if self.service.add_book(isbn, author, title):
                print("Book added!")
            else:
                print("Book couldn't be added! Please try again...")

        if user_command == display_command:
            self.service.display_books()

        if user_command == filter_command:
            word = input("Please give the word: ")
            self.service.filter_books_list(word)
            print("List has been filtered!")

        if user_command == undo_command:
            if self.service.undo():
                print("Last list-modifying operation undone!")
            else:
                print("Undo not possible!")

    def start(self):
        self.print_menu()
        while True:
            user_command = self.get_input()
            if user_command is not None:
                self.process_command(user_command)


if __name__ == "__main__":
    user_interface = UI()
    user_interface.start()
