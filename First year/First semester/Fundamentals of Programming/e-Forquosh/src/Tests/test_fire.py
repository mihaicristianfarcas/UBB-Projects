import unittest

from src.Services.game import Game


class MyTestCase(unittest.TestCase):

    def test_check_if_missile_hit(self):
        game = Game()
        map_entity_list = [(1, 2), (3, 4), (4, 6)]
        self.assertEqual(game.check_if_missile_hit(1, 2, map_entity_list), True)
        self.assertEqual(game.check_if_missile_hit(3, 4, map_entity_list), True)
        self.assertEqual(game.check_if_missile_hit(5, 6, map_entity_list), False)
        map_entity_list = []
        self.assertEqual(game.check_if_missile_hit(1, 1, map_entity_list), False)

    def test_shoot_missile(self):
        game = Game()
        coordinates = 'b3'
        board = game._board
        board._ships = [(0, 3), (1, 6)]
        board._asteroids = [(1, 1), (1, 2), (4, 6), (5, 5)]
        self.assertEqual(game.shoot_missile(coordinates), ("You hit an empty spot.", '0'))
        self.assertEqual(game.shoot_missile('a6'), ("You hit an empty spot.", '0'))
        self.assertEqual(game.shoot_missile('d6'), ("You hit an empty spot.", '0'))
        self.assertEqual(game.shoot_missile('d6'), ("You hit an asteroid or an already hit spot. Try again!", '0'))
        self.assertEqual(game.shoot_missile('b7'), ("You hit an empty spot.", '0'))


if __name__ == '__main__':
    unittest.main()
