#
# This is the program's UI module.
# The user interface and all interaction with the user (print and input statements) are found here.
#
import copy
from functions import *


def display_options() -> None:
    print("\nHello! Please input the command which you want to run (from the ones listed below):\n")
    print("add <number>\ninsert <number> at <position>")
    print("remove <position>\nremove <start position> to <end position>\nreplace <old number> with <new number>")
    print("list\nlist real <start position> to <end position>\nlist modulo [ < | = | > ] <number>")
    print("filter real\nfilter modulo [ < | = | > ] <number>")
    print("undo\n")
    print("0. Exit program.")


def get_user_input():
    return input(">")


def check_valid_user_input_command(user_input_command) -> bool:
    while True:
        if not user_input_command:
            return False
        user_input_command = user_input_command.lower()
        if user_input_command in [list_command, filter_real_command, undo_command]:
            return True
        try:
            user_input_command_split = user_input_command.split()
            assert user_input_command_split[first_command_word] in [add_command, insert_command, remove_command,
                                                                    replace_command, list_command, filter_command,
                                                                    undo_command]
            if len(user_input_command_split) <= 2:
                assert user_input_command_split[first_command_word] in [add_command, remove_command, undo_command]
            if len(user_input_command_split) == 3:
                assert False
            if len(user_input_command_split) == 4:
                if user_input_command_split[first_command_word] == insert_command:
                    assert user_input_command_split[third_command_word] == at_keyword
                if user_input_command_split[first_command_word] == remove_command:
                    assert user_input_command_split[third_command_word] == to_keyword
                if user_input_command_split[first_command_word] == replace_command:
                    assert user_input_command_split[third_command_word] == with_keyword
                if user_input_command_split[first_command_word] in [list_command, filter_command]:
                    assert user_input_command_split[second_command_word] == modulo_keyword
                    assert user_input_command_split[third_command_word] in ['<', '=', '>']
            if len(user_input_command_split) == 5:
                assert user_input_command_split[first_command_word] == list_command
                assert user_input_command_split[second_command_word] == real_keyword
                assert user_input_command_split[fourth_command_word] == to_keyword
            return True
        except AssertionError:
            print("Invalid command. Please try again.")
            return False


def main_menu():

    display_options()
    complex_numbers_list = already_given_complex_numbers_list()
    history_of_commands = []
    complex_numbers_list_copy = copy.deepcopy(complex_numbers_list)
    history_of_commands.append(complex_numbers_list_copy)
    while True:
        user_input_command = get_user_input()
        if user_input_command == exit_command:
            print("Bye!")
            break
        if check_valid_user_input_command(user_input_command):
            user_input_command = user_input_command.split()
            complex_numbers_list = execute_given_command(user_input_command, complex_numbers_list, history_of_commands)
            if complex_numbers_list != complex_numbers_list_copy:
                complex_numbers_list_copy = copy.deepcopy(complex_numbers_list)
                history_of_commands.append(complex_numbers_list_copy)
