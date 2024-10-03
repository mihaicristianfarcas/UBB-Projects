from src.Services.game import Game, GAME_OVER

# constants
FIRE_COMMAND = 'fire'
CHEAT_COMMAND = 'cheat'
COMMAND = 0
COORDINATES = 1


class UI:

    def __init__(self):
        self._game = Game()

    def start(self):
        self._game.generate_asteroids()
        self._game.generate_ships()
        print(self._game.get_board_str())
        while True:
            user_command = input("Enter command: ")
            if self.is_command_valid(user_command) is True:
                user_command = user_command.strip().lower().split()

                if user_command[COMMAND] == FIRE_COMMAND:
                    status_hit, status_game = self._game.shoot_missile(user_command[COORDINATES])
                    if status_game == GAME_OVER:
                        print(status_hit)
                        break
                    else:
                        print(self._game.get_board_str())
                        print(status_hit)

                if user_command[COMMAND] == CHEAT_COMMAND:
                    self._game.get_board_with_ships()
                    print(self._game.get_board_str())

    @staticmethod
    def is_command_valid(user_command):
        try:
            user_command = user_command.strip().lower().split()
            if len(user_command) == 0:
                return False
            if len(user_command) == 1:
                assert  user_command[COMMAND] == CHEAT_COMMAND
            if len(user_command) == 2:
                assert user_command[COMMAND] == FIRE_COMMAND
            return True
        except AssertionError:
            print("Invalid command! Please try again.")
            return False


