"""
Dynamic programming - naive and optimised

3. Given the set of positive integers S, partition this set into two subsets:
    S1, S2 so that the difference between the sum of the elements in S1 and S2 is minimal.
     For example, for set S = { 1, 2, 3, 4, 5 }, the two subsets could be S1 = { 1, 2, 4 } and S2 = { 3, 5 }.
      Display at least one of the solutions.
"""


def find_minimal_difference_of_subarrays_sums(input_array, subarray_1_of_input_array, subarray_2_of_input_array, current_index):
    """
    Finds the two subsets of S (S1 and S2) which have the minimal sum difference using dynamic programming
    :param input_array: The input array, to be divided in S1 and S2
    :param subarray_1_of_input_array: The first subset
    :param subarray_2_of_input_array: The second subset
    :param current_index: The index of the current element
    :return: The minimal difference, subset S1 and subset S2
    """
    if current_index == 0:
        return abs(sum(subarray_1_of_input_array) - sum(subarray_2_of_input_array)), subarray_1_of_input_array, subarray_2_of_input_array

    current_element_of_input_array = input_array[current_index - 1]

    # Adding current_element_of_input_array to the first subset (array_s1)
    difference_of_subarrays_1_and_2_version_1, subarray_1_for_sum_version_1, subarray_2_for_sum_version_1 = find_minimal_difference_of_subarrays_sums(input_array, subarray_1_of_input_array + [current_element_of_input_array], subarray_2_of_input_array,
                                                                                                                                                      current_index - 1)

    # Adding current_element_of_input_array to the second subset (array_s2)
    difference_of_subarrays_1_and_2_version_2, subarray_1_for_sum_version_2, subarray_2_for_sum_version_2 = find_minimal_difference_of_subarrays_sums(input_array, subarray_1_of_input_array, subarray_2_of_input_array + [current_element_of_input_array],
                                                                                                                                                      current_index - 1)

    if difference_of_subarrays_1_and_2_version_1 < difference_of_subarrays_1_and_2_version_2:
        print("Intermediate result: ", difference_of_subarrays_1_and_2_version_1, subarray_1_for_sum_version_1, subarray_2_for_sum_version_1)
        return difference_of_subarrays_1_and_2_version_1, subarray_1_for_sum_version_1, subarray_2_for_sum_version_1
    else:
        return difference_of_subarrays_1_and_2_version_2, subarray_1_for_sum_version_2, subarray_2_for_sum_version_2


def partitions_into_two_subsets_with_minimal_sum_difference_dynamic_programming(input_array):
    """
    Returns the two subsets S1 and S2 which have the minimal sum difference using optimised implementation
    :param input_array: The input array, to be divided in S1 and S2
    :return: S1 and S2
    """
    minimal_difference, subarray_1_of_input_array, subarray_2_of_input_array = find_minimal_difference_of_subarrays_sums(input_array, [], [], len(input_array))
    return subarray_1_of_input_array, subarray_2_of_input_array


def generate_all_combinations_of_input_array(input_array: list):
    import itertools
    all_combinations_with_elements_from_input_array = []
    for i in range(1, len(input_array) + 1):
        all_combinations_with_elements_from_input_array.extend(itertools.combinations(input_array, i))
    return all_combinations_with_elements_from_input_array


def partitions_into_two_subsets_with_minimal_sum_difference_naive(input_array: list):
    """
    Returns the two subsets S1 and S2 which have the minimal sum difference using the naive implementation
    :param input_array: The input array, to be divided in S1 and S2
    :return: S1 and S2
    """
    minimal_difference_of_subarray_1_and_subarray_2 = float('inf')
    best_partitions_of_input_array = None
    all_combinations_of_s = generate_all_combinations_of_input_array(input_array)

    for current_combination in all_combinations_of_s:
        subarray_1_of_input_array = list(current_combination)
        subarray_2_of_input_array = []
        remaining_used_elements_in_subarray_1_from_input_array = len(subarray_1_of_input_array)

        for i in range(len(input_array)):
            if input_array[i] in subarray_1_of_input_array and remaining_used_elements_in_subarray_1_from_input_array:
                remaining_used_elements_in_subarray_1_from_input_array -= 1
            else:
                subarray_2_of_input_array.append(input_array[i])

        difference_of_sums_subarray_1_and_subarray_2 = abs(sum(subarray_1_of_input_array) - sum(subarray_2_of_input_array))
        if difference_of_sums_subarray_1_and_subarray_2 < minimal_difference_of_subarray_1_and_subarray_2:
            minimal_difference_of_subarray_1_and_subarray_2 = difference_of_sums_subarray_1_and_subarray_2
            best_partitions_of_input_array = (subarray_1_of_input_array, subarray_2_of_input_array)

    return best_partitions_of_input_array


def main_menu():
    print("Hello! Please choose an option:")
    print("1. Add values to the set S!")
    print("2. Find two subarrays of S with minimal sum difference using naive implementation!")
    print("3. Find two subarrays of S with minimal sum difference using dynamic programming!")
    print("0. Exit program.")
    generated_input_array = 0
    input_array = []
    while True:
        user_choice = input(">")
        if user_choice in ['1', '2', '3', '0']:
            if user_choice == '1':
                number_of_elements_of_input_array = int(input("Please give the number of elements: "))
                print("Insert values:")
                index = 0
                input_array = []
                while index < number_of_elements_of_input_array:
                    element_of_input_array = int(input(">"))
                    input_array.append(element_of_input_array)
                    index += 1
                print("Array S: ", input_array)
                generated_input_array = 1
            if user_choice == '0':
                print("Bye!")
                break
            if generated_input_array == 1:
                if user_choice == '2':
                    print(partitions_into_two_subsets_with_minimal_sum_difference_naive(input_array))
                if user_choice == '3':
                    print(partitions_into_two_subsets_with_minimal_sum_difference_dynamic_programming(input_array))
                    print("The last one is the final version of the best possible result!")
            else:
                print("Error : Set S not generated!")
        else:
            print("Invalid input value!")


main_menu()
