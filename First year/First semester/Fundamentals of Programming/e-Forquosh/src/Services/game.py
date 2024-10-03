import random

from src.Domain.board import Board

GAME_OVER = '1'
GAME_NOT_OVER = '0'


class Game:

    def __init__(self):
        self._board = Board()

    def get_board_str(self):
        return self._board.__str__()

    def generate_coordinates(self):
        row, column = random.randint(0, self._board.rows - 1), random.randint(0, self._board.columns - 1)
        return row, column

    def generate_ships(self, number_of_ships: int = 2):
        i = 0
        while i < number_of_ships:
            row, column = self.generate_coordinates()
            if self._board.is_spot_empty(row, column) and (row in [0, 6] or column in [0, 6]):
                self._board.place_ships(row, column)
                i += 1

    def generate_asteroids(self, number_of_asteroids: int = 8):
        i = 0
        while i < number_of_asteroids:
            row, column = self.generate_coordinates()
            if self._board.is_spot_empty(row, column):
                self._board.place_asteroid(row, column)
                i += 1

    def get_board_with_ships(self):
        self._board.show_ships()

    def shoot_missile(self, coordinates):
        """
        Shoots a missile at the coordinates given by the player
        If the missile hits a ship, the ship is removed from the board and the hit spot is marked with a '-'
        If the missile hits an asteroid or an empty spot, the ships don't move, so the player can try again
        If the missile hits an empty spot, the game continues normally and the hit spot in marked with a '-'
        :param coordinates: the coordinates of the missile hit
        :return: status of the hit (whether it hit a ship, an asteroid or an already hit spot, or an empty spot)
                    and the status of the game (if all the ships have been hit the game ends)
        """

        row, column = int(coordinates[1]) - 1, coordinates[0]
        column = ord(column.upper()) - ord('A')

        ships = self._board.ships
        asteroids = self._board.asteroids
        hit_spots = self._board.hit_spots

        if self.check_if_missile_hit(row, column, ships):
            for i in range(len(ships)):
                if (row, column) == ships[i]:
                    ships.pop(i)
                    if len(ships) == 0:
                        return "All ships have been destroyed! Game over!", GAME_OVER
                    self._board.mark_hit_spot(row, column)
                    self._board.move_ships()
                    return "You hit a ship! Good job!", GAME_NOT_OVER

        elif self.check_if_missile_hit(row, column, asteroids) or self.check_if_missile_hit(row, column, hit_spots):
            return "You hit an asteroid or an already hit spot. Try again!", GAME_NOT_OVER

        self._board.mark_hit_spot(row, column)
        self._board.move_ships()
        return "You hit an empty spot.", GAME_NOT_OVER

    @staticmethod
    def check_if_missile_hit(row, column, map_entity_list):
        """
        Checks whether the coordinates of the missile coincide with the coordinates of a map entity (asteroid, ship) on the board
        :param map_entity_list: the list of the map entities where we search for the given coordinates of the entity
        :param row: the row coordinate of the missile, input by the player
        :param column: the column coordinate of the missile, input by the player
        :return: True if the coordinates coincide (if the player hit a map entity), False otherwise

        """

        if (row, column) in map_entity_list:
            return True
        return False
