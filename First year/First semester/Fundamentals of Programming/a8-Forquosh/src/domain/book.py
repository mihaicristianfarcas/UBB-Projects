class Book:
    def __init__(self, isbn, author, title):
        if not isinstance(isbn, int):
            raise TypeError()
        if not isinstance(author, str):
            raise TypeError()
        if not isinstance(title, str):
            raise TypeError()

        self._isbn = isbn
        self._author = author
        self._title = title

    @property
    def isbn(self):
        return self._isbn

    @property
    def author(self):
        return self._author

    @property
    def title(self):
        return self._title

    @author.setter
    def author(self, new_author):
        self._author = new_author

    @title.setter
    def title(self, new_title):
        self._title = new_title

    def __str__(self):
        return f'ISBN: {self._isbn} ; Author: {self._author} ; Title: {self._title}\n'

    def __eq__(self, other) -> bool:
        if not isinstance(other, Book):
            return False
        return self.isbn == other.isbn
