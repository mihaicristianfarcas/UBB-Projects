import random


def user_option_processing() -> None:
    """
    Gives instructions to the rest of the program depending on user's input
    :return: None
    """
    generated_numbers = 0
    sequence = []
    while True:
        option = input(">")
        if option in ["1", "2", "3", "4"]:
            if option == "1":
                number_of_elements = int(input("Please give the number of elements:"))
                sequence = generate_random_numbers(number_of_elements)
                print("Sequence:", sequence)
                generated_numbers = 1
            if option == "4":
                print("Bye!")
                break
            if generated_numbers == 1:
                if option == "2" and generated_numbers == 1:
                    print("Please give a step:")
                    step = int(input(">"))
                    bubble_sort(sequence, step)
                    generated_numbers = 0
                if option == "3":
                    print("Please give a step:")
                    step = int(input(">"))
                    gnome_sort(sequence, step)
                    generated_numbers = 0
            else:
                print("Error : No sequence generated or already sorted")
        else:
            print("Error : Please insert valid value")


def generate_random_numbers(how_many_elements: int) -> list:
    """
    Generates a sequence of (how_many_elements) random numbers
    :param how_many_elements: user's desired number of elements
    :return: the generated sequence
    """
    sequence = random.choices(range(100), weights=None, cum_weights=None, k=how_many_elements)
    return sequence


def bubble_sort(sequence: list, step: int):
    """
    Sorts the sequence using bubble sort
    :param sequence: numbers to be sorted
    :param step: how frequently the sequence is printed in the progress of sorting
    :return: the sorted sequence
    """
    is_sorted = 0
    number_of_exchanges_bubble = 0
    while is_sorted != 1:
        is_sorted = 1
        for i in range(len(sequence) - 1):
            if sequence[i] > sequence[i+1]:
                auxiliary_element_bubble = sequence[i]
                sequence[i] = sequence[i+1]
                sequence[i+1] = auxiliary_element_bubble
                is_sorted = 0
                number_of_exchanges_bubble += 1
            if number_of_exchanges_bubble == step:
                print("Current progress:", sequence)
                number_of_exchanges_bubble = 0
    print("Sorted sequence:", sequence)


def gnome_sort(sequence: list, step: int):
    """
    Sorts the sequence using gnome sort
    :param sequence: numbers to be sorted
    :param step: how frequently the sequence is printed in the progress of sorting
    :return: the sorted sequence
    """
    number_of_exchanges_gnome = 0
    i = 0
    while i < len(sequence):
        if i == 0:
            i += 1
        if sequence[i] >= sequence[i - 1]:
            i += 1
        else:
            auxiliary_element_gnome = sequence[i]
            sequence[i] = sequence[i - 1]
            sequence[i - 1] = auxiliary_element_gnome
            i -= 1
            number_of_exchanges_gnome += 1
            if step == number_of_exchanges_gnome:
                print("Current progress:", sequence)
                number_of_exchanges_gnome = 0
    print("Sorted sequence:", sequence)


print("Hello! - Please choose an option:")
print("1. Generate a list of random natural numbers")
print("2. Sort the list using the first algorithm")
print("3. Sort the list using the second algorithm")
print("4. Exit the program")
user_option_processing()
