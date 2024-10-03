from unittest import TestCase
from src.domain.client import Client


class ClientConstructorTest(TestCase):
    def test_client_constructor(self):
        self.__client = Client(1234, "Mihai")
        self.assertEqual(self.__client._id, 1234)
        self.assertEqual(self.__client.name, "Mihai")

    def test_client_constructor_invalid_id(self):
        with self.assertRaises(TypeError):
            Client("1234", "Mihai")

    def test_client_constructor_invalid_name(self):
        with self.assertRaises(TypeError):
            Client(1234, 9101112)

    def test_client_eq_invalid_type(self):
        book = Client(1234, "Mihai")
        self.assertNotEqual(book, 'a')
