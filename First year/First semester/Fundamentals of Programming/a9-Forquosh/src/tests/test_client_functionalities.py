import unittest
from unittest import TestCase

from src.services.client_services import ClientServices


class ClientFunctionalitiesTest(TestCase):

    def test_add_client(self):
        test = ClientServices()
        self.assertEqual(len(test.client_repository.clients_list), 20)
        self.assertEqual(len(test.client_repository.history), 0)
        test.add_client(1928, "Mircea_Eliade")
        self.assertEqual(len(test.client_repository.clients_list), 21)
        self.assertEqual(len(test.client_repository.history), 1)
        test.add_client(1002, "da")
        self.assertEqual(len(test.client_repository.clients_list), 21)
        self.assertEqual(len(test.client_repository.history), 1)

    def test_remove_client(self):
        test = ClientServices()
        self.assertEqual(len(test.client_repository.clients_list), 20)
        self.assertEqual(len(test.client_repository.history), 0)
        test.remove_client(1002)
        self.assertEqual(len(test.client_repository.clients_list), 19)
        self.assertEqual(len(test.client_repository.history), 1)

    def test_update_book(self):
        test = ClientServices()
        test.update_client(1000,"Mircea_Eliade")
        self.assertEqual(test.client_repository.clients_list[0].name, "Mircea_Eliade")


if __name__ == '__main__':
    unittest.main()
