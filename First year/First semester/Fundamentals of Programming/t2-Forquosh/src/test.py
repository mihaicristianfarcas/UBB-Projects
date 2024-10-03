import unittest

from src.services import Services


class MyTestCase(unittest.TestCase):
    def test_third_functionality(self):
        service = Services()
        service.repository.load_file()
        bus_id = 1
        self.assertEqual(bus_id, 1)
        bus, distance = service.compute_distance(1)
        self.assertEqual(distance, 30)
        self.assertEqual(bus.route_code, 1)
        self.assertEqual(bus.times_on_route, 3)


if __name__ == '__main__':
    unittest.main()
