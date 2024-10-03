#
# The program's functions are implemented here. There is no user interaction in this file,
# therefore no input/print statements. Functions here communicate via function parameters,
# the return statement and raising of exceptions.
#
from math import sqrt

first_command_word = 0
second_command_word = 1
third_command_word = 2
fourth_command_word = 3
fifth_command_word = 4

imaginary_part_constant = 1
real_part_constant = 0

add_command = "add"
insert_command = "insert"
remove_command = "remove"
replace_command = "replace"
list_command = "list"
filter_command = "filter"
filter_real_command = "filter real"
undo_command = "undo"
exit_command = "0"
at_keyword = "at"
with_keyword = "with"
to_keyword = "to"
modulo_keyword = "modulo"
real_keyword = "real"


def already_given_complex_numbers_list():
    complex_numbers_list = [create_complex_number_list(1, 2), create_complex_number_list(3, 4),
                            create_complex_number_list(5, 6), create_complex_number_list(7, 8),
                            create_complex_number_list(5, 6), create_complex_number_list(4, 3),
                            create_complex_number_list(7, 2), create_complex_number_list(1, 8),
                            create_complex_number_list(4, 3), create_complex_number_list(3, 4)]

    return complex_numbers_list


def create_complex_number_list(real_part: int, imaginary_part: int) -> list:
    return [real_part, imaginary_part]


def convert_complex_number_from_list_to_string(complex_number_list) -> str:
    """
    Converts the complex number to a string and returns it
    :param complex_number_list: the complex number as a list
    :return: the complex number as a string
    """
    if complex_number_list[imaginary_part_constant] == 0:
        return str(complex_number_list[real_part_constant])
    if complex_number_list[real_part_constant] == 0:
        return str(complex_number_list[imaginary_part_constant]) + "i"
    if complex_number_list[imaginary_part_constant] < 0:
        return str(complex_number_list[real_part_constant]) + str(complex_number_list[imaginary_part_constant]) + "i"
    return str(complex_number_list[real_part_constant]) + "+" + str(complex_number_list[imaginary_part_constant]) + "i"


def extract_imaginary_part_from_string(imaginary_part: str) -> int:
    imaginary_part = imaginary_part.replace("i", "")
    try:
        imaginary_part = int(imaginary_part)
    except ValueError:
        imaginary_part = 1
    return imaginary_part


def separate_complex_number_string(complex_number_as_string: str) -> list:
    """
    Converts a string to a complex number by taking the value of the real part and imaginary part
    :param complex_number_as_string: the complex number as a string
    :return: the two values, real part and imaginary part
    """
    real_part = 0
    imaginary_part = 0
    is_real_part_negative = False
    is_imaginary_part_negative = False

    if complex_number_as_string.startswith("-"):
        is_real_part_negative = True
        complex_number_as_string = complex_number_as_string[1:]

    if "+" not in complex_number_as_string and "-" not in complex_number_as_string:
        if "i" in complex_number_as_string:
            complex_number_as_string = extract_imaginary_part_from_string(complex_number_as_string)
            if is_real_part_negative:
                complex_number_as_string = complex_number_as_string * (-1)
            return [0, int(complex_number_as_string)]
        else:
            complex_number_as_string = int(complex_number_as_string)
            if is_real_part_negative:
                complex_number_as_string = complex_number_as_string * (-1)
            return [int(complex_number_as_string), 0]
    elif "+" in complex_number_as_string:
        real_part, imaginary_part = complex_number_as_string.split("+")
    elif "-" in complex_number_as_string:
        real_part, imaginary_part = complex_number_as_string.split("-")
        is_imaginary_part_negative = True

    if "i" in imaginary_part:
        real_part = int(real_part)
        imaginary_part = extract_imaginary_part_from_string(imaginary_part)
        if is_imaginary_part_negative:
            imaginary_part = imaginary_part * (-1)
        if is_real_part_negative:
            real_part = real_part * (-1)

    elif "i" in real_part:
        real_part, imaginary_part = imaginary_part, real_part
        real_part = int(real_part)
        imaginary_part = extract_imaginary_part_from_string(imaginary_part)
        if is_imaginary_part_negative:
            real_part = real_part * (-1)
        if is_real_part_negative:
            imaginary_part = imaginary_part * (-1)

    return [int(real_part), int(imaginary_part)]


def add_complex_number_to_list(new_complex_number: str, complex_numbers_list):
    real_part, imaginary_part = separate_complex_number_string(new_complex_number)
    complex_numbers_list.append(create_complex_number_list(real_part, imaginary_part))


def insert_complex_number_at_given_index(new_complex_number: str, index, complex_numbers_list):
    try:
        index = int(index)
        real_part, imaginary_part = separate_complex_number_string(new_complex_number)
        complex_numbers_list.insert(index, create_complex_number_list(real_part, imaginary_part))
    except ValueError:
        print("Index value not valid! Please try again.")


def remove_complex_numbers_from_index1_to_index2(start_position, end_position, complex_numbers_list):
    try:
        start_position = int(start_position)
        end_position = int(end_position)
        assert end_position <= len(complex_numbers_list) and start_position >= 0
        del complex_numbers_list[start_position:end_position]
        return complex_numbers_list
    except (ValueError, AssertionError):
        print("Index values not valid! Please try again.")


def replace_complex_number(old_complex_number, new_complex_number, complex_numbers_list):
    old_complex_number = separate_complex_number_string(old_complex_number)
    if old_complex_number not in complex_numbers_list:
        return print("Old number not in list! Please try again.")
    else:
        index = 0
        for current_complex_number in complex_numbers_list:
            if current_complex_number == old_complex_number:
                break
            index += 1
        remove_complex_numbers_from_index1_to_index2(index, index+1, complex_numbers_list)
        insert_complex_number_at_given_index(new_complex_number, index, complex_numbers_list)


def list_complex_numbers(complex_numbers_list):
    for i in complex_numbers_list:
        print(convert_complex_number_from_list_to_string(i), end="")
        print("")


def list_real_from_index1_to_index2(start_position, end_position, complex_numbers_list):
    try:
        start_position = int(start_position)
        end_position = int(end_position)
        assert end_position <= len(complex_numbers_list) and start_position >= 0
        for i in range(start_position, end_position):
            if complex_numbers_list[i][1] == 0:
                print(complex_numbers_list[i][0])
    except (ValueError, AssertionError):
        print("Index values not valid! Please try again.")


def modulo_of_complex_number(real_part, imaginary_part):
    """
    Computes the module of the complex number
    :param real_part: the real part of the complex number
    :param imaginary_part: the imaginary part of the complex number
    :return: the modulo
    """
    return int(sqrt(real_part*real_part + imaginary_part*imaginary_part))


def list_complex_numbers_by_modulo(symbol, value, complex_numbers_list):
    """
    Prints all values of corresponding modulo with the operation noted by the symbol
    :param symbol: the requested operation
    :param value: value compared with the modulo of the number
    :param complex_numbers_list: the list of complex numbers to be compared
    :return: the numbers which satisfy the given condition
    """
    try:
        value = int(value)
        for i in range(len(complex_numbers_list)):
            real_part = complex_numbers_list[i][0]
            imaginary_part = complex_numbers_list[i][1]
            if symbol == "<":
                if modulo_of_complex_number(real_part, imaginary_part) < value:
                    print(convert_complex_number_from_list_to_string(complex_numbers_list[i]))
            elif symbol == "=":
                if modulo_of_complex_number(real_part, imaginary_part) == value:
                    print(convert_complex_number_from_list_to_string(complex_numbers_list[i]))
            elif symbol == ">":
                if modulo_of_complex_number(real_part, imaginary_part) > value:
                    print(convert_complex_number_from_list_to_string(complex_numbers_list[i]))
    except ValueError:
        print("Value not correct! Please try again.")


def filter_by_real_part(complex_numbers_list):
    """
    Deletes the complex numbers
    :param complex_numbers_list: the list of complex numbers to be filtered
    :return: the remaining real numbers from the list
    """
    i = 0
    while i < len(complex_numbers_list):
        if complex_numbers_list[i][1] != 0:
            del complex_numbers_list[i]
            i -= 1
        i += 1


def filter_complex_numbers_by_modulo(symbol, value, complex_numbers_list):
    """
       Deletes all values of not corresponding modulo with the operation noted by the symbol
       :param symbol: the requested operation
       :param value: value compared with the modulo of the number
       :param complex_numbers_list: the list of complex numbers to be compared
       :return: the numbers which satisfy the given condition
    """
    try:
        value = int(value)
        i = 0
        while i < len(complex_numbers_list):
            real_part = complex_numbers_list[i][0]
            imaginary_part = complex_numbers_list[i][1]
            if symbol == "<":
                if not modulo_of_complex_number(real_part, imaginary_part) < value:
                    del complex_numbers_list[i]
                    i -= 1
            elif symbol == "=":
                if not modulo_of_complex_number(real_part, imaginary_part) == value:
                    del complex_numbers_list[i]
                    i -= 1
            elif symbol == ">":
                if not modulo_of_complex_number(real_part, imaginary_part) > value:
                    del complex_numbers_list[i]
                    i -= 1
            i += 1
    except ValueError:
        print("Value not correct! Please try again.")


def execute_given_command(user_input_command, complex_numbers_list, history_of_commands):
    """
    Executor of the commands
    :param user_input_command: the command given by the user
    :param complex_numbers_list: the list of complex numbers
    :param history_of_commands: the complex numbers, at each stage of the program
    :return: None
    """

    if user_input_command[first_command_word] == add_command:
        if len(user_input_command) > 1:
            add_complex_number_to_list(user_input_command[second_command_word], complex_numbers_list)
        else:
            print("Invalid command! Please try again.")

    elif user_input_command[first_command_word] == insert_command:
        insert_complex_number_at_given_index(user_input_command[second_command_word],
                                             user_input_command[fourth_command_word],
                                             complex_numbers_list)

    elif user_input_command[first_command_word] == remove_command:
        remove_complex_numbers_from_index1_to_index2(user_input_command[second_command_word],
                                                     user_input_command[fourth_command_word],
                                                     complex_numbers_list)

    elif user_input_command[first_command_word] == replace_command:
        replace_complex_number(user_input_command[second_command_word],
                               user_input_command[fourth_command_word],
                               complex_numbers_list)

    elif user_input_command[first_command_word] == list_command:
        if len(user_input_command) > 1:
            if user_input_command[second_command_word] == "real":
                list_real_from_index1_to_index2(user_input_command[third_command_word],
                                                user_input_command[fifth_command_word],
                                                complex_numbers_list)
            else:
                list_complex_numbers_by_modulo(user_input_command[third_command_word],
                                               user_input_command[fourth_command_word],
                                               complex_numbers_list)
        else:
            list_complex_numbers(complex_numbers_list)

    elif user_input_command[first_command_word] == filter_command:
        if len(user_input_command) > 2:
            filter_complex_numbers_by_modulo(user_input_command[third_command_word],
                                             user_input_command[fourth_command_word],
                                             complex_numbers_list)
        else:
            filter_by_real_part(complex_numbers_list)

    elif user_input_command[first_command_word] == undo_command:
        if len(history_of_commands) > 1:
            history_of_commands.pop()
            complex_numbers_list = history_of_commands[len(history_of_commands) - 1]
        else:
            print("Undo not possible!")
        pass
    return complex_numbers_list
