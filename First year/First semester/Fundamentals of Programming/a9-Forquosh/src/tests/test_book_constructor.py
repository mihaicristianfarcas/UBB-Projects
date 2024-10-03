from unittest import TestCase
from src.domain.book import Book


class BookConstructorTest(TestCase):
    def test_book_constructor(self):
        self.__book = Book(1234, "Shakespeare", "Hamlet")
        self.assertEqual(self.__book.isbn, 1234)
        self.assertEqual(self.__book.author, "Shakespeare")
        self.assertEqual(self.__book.title, "Hamlet")

    def test_book_constructor_invalid_isbn(self):
        with self.assertRaises(TypeError):
            Book("1234", "Shakespeare", "Hamlet")

    def test_book_constructor_invalid_author(self):
        with self.assertRaises(TypeError):
            Book(1234, 9101112, "Hamlet")

    def test_book_constructor_invalid_title(self):
        with self.assertRaises(TypeError):
            Book(1234, "Shakespeare", 9101112)

    def test_book_eq_invalid_type(self):
        book = Book(1234, "Shakespeare", "Hamlet")
        self.assertNotEqual(book, 'a')
