#
# Write the implementation for A5 in this file
#

# 
# Write below this comment 
# Functions to deal with complex numbers -- list representation
# -> There should be no print or input statements in this section 
# -> Each function should do one thing only
# -> Functions communicate using input parameters and their return values
#
# Functions that deal with complex numbers


def already_given_complex_numbers_list():
    complex_numbers_list = [create_complex_number_list(1, 2), create_complex_number_list(3, 4),
                            create_complex_number_list(5, 6), create_complex_number_list(7, 8),
                            create_complex_number_list(9, 10), create_complex_number_list(10, 9),
                            create_complex_number_list(7, 8), create_complex_number_list(6, 5),
                            create_complex_number_list(4, 3), create_complex_number_list(3, 4)]

    return complex_numbers_list


def create_complex_number_list(real_part: int, imaginary_part: int) -> list:
    return [real_part, imaginary_part]


def add_complex_numbers_to_list(complex_numbers_list, how_many_new_complex_numbers: int):
    while how_many_new_complex_numbers:
        complex_number_real_part = int(input("Real part: "))
        complex_number_imaginary_part = int(input("Imaginary part: "))
        new_complex_number = create_complex_number_list(complex_number_real_part, complex_number_imaginary_part)
        complex_numbers_list.append(new_complex_number)
        how_many_new_complex_numbers -= 1


def convert_complex_number_from_list_to_string(complex_number_list) -> str:
    if complex_number_list[1] == 0:
        return str(complex_number_list[0])
    if complex_number_list[0] == 0:
        return str(complex_number_list[1]) + "i"
    if complex_number_list[1] < 0:
        return str(complex_number_list[0]) + str(complex_number_list[1]) + "i"
    return str(complex_number_list[0]) + "+" + str(complex_number_list[1]) + "i"


#
# Write below this comment 
# Functions to deal with complex numbers -- dict representation
# -> There should be no print or input statements in this section 
# -> Each function should do one thing only
# -> Functions communicate using input parameters and their return values
#
# Functions for Complex Number using Dictionaries


def already_given_complex_numbers_dictionary():
    complex_numbers_dictionary = [create_complex_number_dictionary(1, 2), create_complex_number_dictionary(3, 4),
                                  create_complex_number_dictionary(5, 6), create_complex_number_dictionary(2, 8),
                                  create_complex_number_dictionary(4, 10), create_complex_number_dictionary(6, 9),
                                  create_complex_number_dictionary(8, 7), create_complex_number_dictionary(9, 5),
                                  create_complex_number_dictionary(4, 3), create_complex_number_dictionary(11, 1)]
    return complex_numbers_dictionary


def create_complex_number_dictionary(real, imaginary):
    return {'real_part': real, 'imaginary_part': imaginary}


def add_complex_numbers_to_dictionary(complex_numbers_dictionary, how_many_new_complex_numbers: int):
    while how_many_new_complex_numbers:
        complex_number_real_part = int(input("Real part: "))
        complex_number_imaginary_part = int(input("Imaginary part: "))
        new_complex_number = create_complex_number_dictionary(complex_number_real_part, complex_number_imaginary_part)
        complex_numbers_dictionary.append(new_complex_number)
        how_many_new_complex_numbers -= 1


def convert_complex_number_from_dictionary_to_string(complex_number_dictionary):
    if complex_number_dictionary['imaginary_part'] == 0:
        return str(complex_number_dictionary['real_part'])
    if complex_number_dictionary['real_part'] == 0:
        return str(complex_number_dictionary['imaginary_part']) + "i"
    if complex_number_dictionary['imaginary_part'] < 0:
        return str(complex_number_dictionary['real_part']) + str(complex_number_dictionary['imaginary_part']) + "i"
    return str(complex_number_dictionary['real_part']) + "+" + str(complex_number_dictionary['imaginary_part']) + "i"


#
# Write below this comment 
# Functions that deal with subarray/subsequence properties
# -> There should be no print or input statements in this section 
# -> Each function should do one thing only
# -> Functions communicate using input parameters and their return values
#


def longest_subarray_of_distinct_numbers(complex_numbers_list):
    longest_subarray = []
    current_subarray = []
    current_number_of_elements = 0
    longest_subarray_number_of_elements = -1

    for i in range(len(complex_numbers_list)):
        if complex_numbers_list[i] in current_subarray:
            if longest_subarray_number_of_elements > current_number_of_elements:
                current_subarray = []
                current_number_of_elements = 0
            else:
                longest_subarray = current_subarray.copy()
                longest_subarray_number_of_elements = current_number_of_elements
                current_subarray = []
                current_number_of_elements = 0
        else:
            current_subarray.append(complex_numbers_list[i])
            current_number_of_elements += 1
    if longest_subarray_number_of_elements < current_number_of_elements:
        longest_subarray = current_subarray
        longest_subarray_number_of_elements = current_number_of_elements

    return longest_subarray, longest_subarray_number_of_elements


def longest_increasing_subsequence_by_real_part(complex_numbers):

    max_length_of_increasing_subsequence = 1
    best_end_of_increasing_subsequence = 0
    indexes_array = [1]
    previous_indexes_array = [-1]

    for i in range(1, len(complex_numbers)):
        indexes_array.append(1)
        previous_indexes_array.append(-1)

        for j in range(i - 1, -1, -1):
            if type(complex_numbers[j]) is list:
                if (indexes_array[j] + 1 > indexes_array[i]) and (complex_numbers[j][0] < complex_numbers[i][0]):
                    indexes_array[i] = indexes_array[j] + 1
                    previous_indexes_array[i] = j
            elif type(complex_numbers[j]) is dict:
                if (indexes_array[j] + 1 > indexes_array[i]) and (complex_numbers[j]['real_part'] < complex_numbers[i]['real_part']):
                    indexes_array[i] = indexes_array[j] + 1
                    previous_indexes_array[i] = j

        if indexes_array[i] > max_length_of_increasing_subsequence:
            best_end_of_increasing_subsequence = i
            max_length_of_increasing_subsequence = indexes_array[i]

    longest_increasing_subsequence = [complex_numbers[best_end_of_increasing_subsequence]]
    while previous_indexes_array[best_end_of_increasing_subsequence] != -1:
        longest_increasing_subsequence.append(complex_numbers[previous_indexes_array[best_end_of_increasing_subsequence]])
        best_end_of_increasing_subsequence = previous_indexes_array[best_end_of_increasing_subsequence]

    longest_increasing_subsequence.reverse()
    return longest_increasing_subsequence


#
# Write below this comment 
# UI section
# Write all functions that have input or print statements here
# Ideally, this section should not contain any calculations relevant to program functionalities
#


def display_all_complex_numbers_from_list(complex_numbers_list):
    for i in complex_numbers_list:
        print(convert_complex_number_from_list_to_string(i), end="")
        print("")


def display_all_complex_numbers_from_dictionary(complex_numbers_dictionary):
    for i in complex_numbers_dictionary:
        print(convert_complex_number_from_dictionary_to_string(i), end="")
        print("")


def display_longest_subarray_of_distinct_numbers(elements_of_subarray, number_of_elements):
    if type(elements_of_subarray[0]) is list:
        display_all_complex_numbers_from_list(elements_of_subarray)
        print("The number of elements of the longest subarray of distinct elements: ", number_of_elements)
    elif type(elements_of_subarray[0]) is dict:
        display_all_complex_numbers_from_dictionary(elements_of_subarray)
        print("The number of elements of the longest subarray of distinct elements: ", number_of_elements)


def main_menu():
    print("Hello! Please choose an option:")
    print("1. Add and save complex numbers!")
    print("2. Display the entire list of numbers on the console!")
    print("3. Display the length and elements of the longest subarray of distinct numbers!")
    print("4. Display the The length and elements of a longest increasing subsequence, when considering each number's real part!")
    print("0. Exit program.")

    # Initial 10 complex numbers in list and dictionary
    complex_numbers_list = already_given_complex_numbers_list()
    complex_numbers_dictionary = already_given_complex_numbers_dictionary()

    # Options:
    add_complex_numbers = '1'
    display_complex_numbers = '2'
    display_length_and_elements_of_longest_subarray_of_distinct_numbers = '3'
    display_elements_of_longest_increasing_subsequence_by_real_part = '4'
    exit_program = '0'

    # Options for list or dictionary
    list_implementation = 1
    dictionary_implementation = 2

    while True:
        user_choice = input(">")
        if user_choice in [exit_program, add_complex_numbers, display_complex_numbers, display_length_and_elements_of_longest_subarray_of_distinct_numbers, display_elements_of_longest_increasing_subsequence_by_real_part]:

            if user_choice == add_complex_numbers:
                option = int(input("Please choose to which data structure you want to add the number (1 - list ; 2 - dictionary): "))
                if option == list_implementation:
                    how_many_new_complex_numbers = int(input("Please specify how many numbers you want to add to the list: "))
                    add_complex_numbers_to_list(complex_numbers_list, how_many_new_complex_numbers)
                elif option == dictionary_implementation:
                    how_many_new_complex_numbers = int(input("Please specify how many numbers you want to add to the dictionary: "))
                    add_complex_numbers_to_dictionary(complex_numbers_dictionary, how_many_new_complex_numbers)
                else:
                    print("Invalid value!")

            if user_choice == display_complex_numbers:
                option = int(input("Please choose which data structure to display (1 - list ; 2 - dictionary): "))
                if option == list_implementation:
                    display_all_complex_numbers_from_list(complex_numbers_list)
                elif option == dictionary_implementation:
                    display_all_complex_numbers_from_dictionary(complex_numbers_dictionary)
                else:
                    print("Invalid value!")

            if user_choice == display_length_and_elements_of_longest_subarray_of_distinct_numbers:
                option = int(input("Please choose with which data structure to work (1 - list ; 2 - dictionary): "))
                if option == list_implementation:
                    longest_subarray, number_of_elements = longest_subarray_of_distinct_numbers(complex_numbers_list)
                    display_longest_subarray_of_distinct_numbers(longest_subarray, number_of_elements)
                elif option == dictionary_implementation:
                    longest_subarray, number_of_elements = longest_subarray_of_distinct_numbers(complex_numbers_dictionary)
                    display_longest_subarray_of_distinct_numbers(longest_subarray, number_of_elements)
                else:
                    print("Invalid value!")

            if user_choice == display_elements_of_longest_increasing_subsequence_by_real_part:
                option = int(input("Please choose with which data structure to work (1 - list ; 2 - dictionary): "))
                if option == list_implementation:
                    longest_increasing_subsequence = longest_increasing_subsequence_by_real_part(complex_numbers_list)
                    display_all_complex_numbers_from_list(longest_increasing_subsequence)
                elif option == dictionary_implementation:
                    longest_increasing_subsequence = longest_increasing_subsequence_by_real_part(complex_numbers_dictionary)
                    display_all_complex_numbers_from_dictionary(longest_increasing_subsequence)
                else:
                    print("Invalid value!")
            if user_choice == exit_program:
                print("Bye!")
                break
        else:
            print("Invalid input value!")


main_menu()
