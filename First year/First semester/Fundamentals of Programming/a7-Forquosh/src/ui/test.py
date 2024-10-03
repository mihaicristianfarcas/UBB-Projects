import unittest
from src.services.services import Services


class MyTestCase(unittest.TestCase):

    def test_add_book(self):
        test = Services()
        self.assertEqual(len(test.repository.books_list), 10)
        test.add_book(1928, "Mircea_Eliade", "Sacrul_si_Profanul")
        self.assertEqual(len(test.repository.books_list), 11)
        test.add_book(1234, "da", "nu")
        self.assertEqual(len(test.repository.books_list), 11)

    def test_filter_books_list(self):
        test = Services()
        self.assertEqual(len(test.repository.books_list), 10)
        self.assertEqual(len(test.repository.history), 0)
        test.repository.filter("The")
        self.assertEqual(len(test.repository.books_list), 5)
        self.assertEqual(len(test.repository.history), 1)

    def test_undo(self):
        test = Services()
        self.assertEqual(len(test.repository.books_list), 10)
        self.assertEqual(len(test.repository.history), 0)
        test.repository.filter("The")
        self.assertEqual(len(test.repository.books_list), 5)
        self.assertEqual(len(test.repository.history), 1)
        test.undo()
        assert len(test.repository.books_list) == 10
        assert len(test.repository.history) == 0


if __name__ == '__main__':
    unittest.main()
