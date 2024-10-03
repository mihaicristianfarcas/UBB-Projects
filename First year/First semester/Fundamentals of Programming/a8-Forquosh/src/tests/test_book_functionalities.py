import unittest
from unittest import TestCase
from src.services.book_services import BookServices


class BookFunctionalitiesTest(TestCase):

    def test_add_book(self):
        test = BookServices()
        self.assertEqual(len(test.book_repository.books_list), 20)
        test.add_book(1928, "Mircea_Eliade", "Sacrul_si_Profanul")
        self.assertEqual(len(test.book_repository.books_list), 21)
        test.add_book(1002, "da", "nu")
        self.assertEqual(len(test.book_repository.books_list), 21)

    def test_remove_book(self):
        test = BookServices()
        self.assertEqual(len(test.book_repository.books_list), 20)
        self.assertEqual(len(test.book_repository.history), 0)
        test.book_repository.remove(1002)
        self.assertEqual(len(test.book_repository.books_list), 19)
        self.assertEqual(len(test.book_repository.history), 1)

    def test_update_book(self):
        test = BookServices()
        test.update_book(1000,'A',"Mircea_Eliade")
        self.assertEqual(test.book_repository.books_list[0].author, "Mircea_Eliade")

        test.update_book(1000, 'B', "Sacrul_si_Profanul")
        self.assertEqual(test.book_repository.books_list[0].title, "Sacrul_si_Profanul")


if __name__ == '__main__':
    unittest.main()
