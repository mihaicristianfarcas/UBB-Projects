import random
import time


def worst_case_array_generator_and_timer(initial_array_length: int):
    current_array_length = initial_array_length
    algorithm_of_choice = int(input("Please select algorithm (1 - Bubble , 2 - Gnome): "))
    for i in range(1, 6):
        array_for_worst_case = list(reversed(range(current_array_length)))
        print("Current step:", i, '(', current_array_length, 'elements )')
        if algorithm_of_choice == 1:
            bubble_sort(array_for_worst_case, -1)
        else:
            gnome_sort(array_for_worst_case, -1)
        current_array_length *= 2


def average_case_array_generator_and_timer(initial_array_length: int):
    algorithm_of_choice = int(input("Please select algorithm (1 - Bubble , 2 - Gnome): "))
    current_array_length = initial_array_length
    for i in range(1, 6):
        array_for_average_case = generate_random_numbers(current_array_length)
        print("Current step:", i, '(', current_array_length, 'elements )')
        if algorithm_of_choice == 1:
            bubble_sort(array_for_average_case, -1)
        else:
            gnome_sort(array_for_average_case, -1)
        current_array_length *= 2


def best_case_array_generator_and_timer(initial_array_length: int):
    current_array_length = initial_array_length
    algorithm_of_choice = int(input("Please select algorithm (1 - Bubble , 2 - Gnome): "))
    for i in range(1, 6):
        array_for_best_case = list(range(current_array_length))
        print("Current step:", i, '(', current_array_length, 'elements )')
        if algorithm_of_choice == 1:
            bubble_sort(array_for_best_case, -1)
        else:
            gnome_sort(array_for_best_case, -1)
        current_array_length *= 2


def user_option_processing() -> None:
    """
    Gives instructions to the rest of the program depending on user's input
    :return: None
    """
    generated_numbers = 0
    random_numbers = []
    while True:
        option = input(">")
        if option in ["1", "2", "3", "4", "5", "6", "0"]:

            if option == "1":
                number_of_elements = int(input("Please give the number of elements:"))
                random_numbers = generate_random_numbers(number_of_elements)
                print("Array:", random_numbers)
                generated_numbers = 1

            if option == "0":
                print("Bye!")
                break

            if option == "4":
                print("On worst case, the time complexity for both these algorithms is O(n * n).")
                print("The input array is already sorted, but in reverse order.")
                initial_array_length = int(input("Please give the initial array length: "))
                print("First array length is", initial_array_length, ", doubling at each new step.")
                worst_case_array_generator_and_timer(initial_array_length)

            if option == "5":
                print("On average, the time complexity for both these algorithms is O(n * n).")
                initial_array_length = int(input("Please give the initial array length: "))
                print("First array length is", initial_array_length, ", doubling at each new step.")
                average_case_array_generator_and_timer(initial_array_length)

            if option == "6":
                print("On the best case, the time complexity for both these algorithms is O(n).")
                print("The input array is already sorted.")
                initial_array_length = int(input("Please give the initial array length: "))
                print("First array length is", initial_array_length, ", doubling at each new step.")
                best_case_array_generator_and_timer(initial_array_length)

            if generated_numbers == 1:
                if option == "2" and generated_numbers == 1:
                    print("Please give a step:")
                    step = int(input(">"))
                    bubble_sort(random_numbers, step)
                    generated_numbers = 0

                if option == "3":
                    print("Please give a step:")
                    step = int(input(">"))
                    gnome_sort(random_numbers, step)
                    generated_numbers = 0

            elif int(option) < 4:
                print("Error : No array generated or already sorted.")
        else:
            print("Error : Please insert valid value.")


def generate_random_numbers(how_many_elements: int) -> list:
    """
    Generates an array of (how_many_elements) random numbers
    :param how_many_elements: user's desired number of elements
    :return: the generated array
    """
    randomly_generated_array = random.choices(range(100), weights=None, cum_weights=None, k=how_many_elements)
    return randomly_generated_array


def bubble_sort(array_to_be_sorted: list, step: int):
    """
    Sorts the array using bubble sort
    :param array_to_be_sorted: numbers to be sorted
    :param step: how frequently the array is printed in the progress of sorting
    :return: the sorted array
    """
    is_sorted = 0
    number_of_exchanges = 0
    start_time = time.perf_counter()
    while is_sorted != 1:
        is_sorted = 1
        for i in range(len(array_to_be_sorted) - 1):
            if array_to_be_sorted[i] > array_to_be_sorted[i + 1]:
                temporarily_stored_element_for_swap = array_to_be_sorted[i]
                array_to_be_sorted[i] = array_to_be_sorted[i + 1]
                array_to_be_sorted[i + 1] = temporarily_stored_element_for_swap
                is_sorted = 0
                number_of_exchanges += 1
            if number_of_exchanges == step:
                print("Current progress:", array_to_be_sorted)
                number_of_exchanges = 0
    end_time = time.perf_counter()
    if step != -1:
        print("Sorted array:", array_to_be_sorted)
    else:
        print("Execution time for BubbleSort: ", end_time - start_time)


def gnome_sort(array_to_be_sorted: list, step: int):
    """
    Sorts the array using gnome sort
    :param array_to_be_sorted: numbers to be sorted
    :param step: how frequently the array is printed in the progress of sorting
    :return: the sorted array
    """
    number_of_exchanges = 0
    i = 0
    start_time = time.perf_counter()
    while i < len(array_to_be_sorted):
        if i == 0:
            i += 1
        if array_to_be_sorted[i] >= array_to_be_sorted[i - 1]:
            i += 1
        else:
            temporarily_stored_element_for_swap = array_to_be_sorted[i]
            array_to_be_sorted[i] = array_to_be_sorted[i - 1]
            array_to_be_sorted[i - 1] = temporarily_stored_element_for_swap
            i -= 1
            number_of_exchanges += 1
            if step == number_of_exchanges:
                print("Current progress:", array_to_be_sorted)
                number_of_exchanges = 0
    end_time = time.perf_counter()
    if step != -1:
        print("Sorted array:", array_to_be_sorted)
    else:
        print("Execution time for GnomeSort: ", end_time - start_time)


print("Hello! - Please choose an option:")
print("1. Generate an array of random natural numbers")
print("2. Sort the array using the BubbleSort algorithm")
print("3. Sort the array using the GnomeSort algorithm")
print("4. WORST CASE - BubbleSort and GnomeSort")
print("5. AVERAGE CASE - BubbleSort and GnomeSort")
print("6. BEST CASE - BubbleSort and GnomeSort")
print("0. Exit the program")
user_option_processing()
