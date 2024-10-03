from src.domain.isbn_object import ISBNObject


class Books(ISBNObject):
    def __init__(self, isbn, author: str, title: str):
        super().__init__(isbn)
        self.author = author
        self.title = title

    def __str__(self):
        return f'ISBN: {self.isbn} ; Author: {self.author} ; Title: {self.title}\n'

    def set_author(self, author):
        self.author = author

    def set_title(self, title):
        self.title = title
