from texttable import Texttable


class Board:

    def __init__(self, rows: int = 7, columns: int = 7):
        self._rows = rows
        self._columns = columns
        self._board = [[' ' for _ in range(self._rows)] for _ in range(self._columns)]
        self._board[self._rows // 2][self._columns // 2] = 'E'
        self._asteroids = []
        self._ships = []
        self._hit_spots = []

    @property
    def rows(self):
        return self._rows

    @property
    def columns(self):
        return self._columns

    @property
    def asteroids(self):
        return self._asteroids

    @property
    def board(self):
        return self._board

    @property
    def ships(self):
        return self._ships

    @property
    def hit_spots(self):
        return self._hit_spots

    def place_asteroid(self, row, column):
        self._board[row][column] = '*'
        self._asteroids.append((row, column))

    def place_ships(self, row, column):
        self._ships.append((row, column))

    def is_spot_empty(self, row, column):
        return True if self._board[row][column] == ' ' else False

    def __str__(self):
        display_board = Texttable()
        header = [' ']
        for i in range(self._columns):
            header.append(chr(ord('A') + i))
        display_board.header(header)

        for i in range(self._rows):
            display_board.add_row([i + 1] + self._board[i])

        return display_board.draw()

    def show_ships(self):
        for ship in self._ships:
            self._board[ship[0]][ship[1]] = 'X'

    def mark_hit_spot(self, row, column):
        """
        Marks the spot at the row, column coordinates with an '-'
        :param row: the row coordinate
        :param column: the column coordinate
        :return: None
        """
        self._board[row][column] = '-'
        self._hit_spots.append((row, column))

    def move_ships(self):
        """
        Moves the ships on the board
        :return: None
        """
        pass
