"""
Backtracking - iterative and recursive

4. A player at `PRONOSPORT` wants to choose score options for four games. The options may be `1`, `X`, `2`.
    Generate all possible alternatives, knowing that:
        - The last score option may not be `X`
        - There should be no more than two score options of `1`
"""


def generate_pronosport_bet_alternatives_iterative():
    """
    Generates all bet bet_alternatives in the iterative way using BACKTRACKING
    :return: all bet bet_alternatives, in a list
    """
    bet_alternatives = []
    stack = [(0, [])]
    while stack:
        index, current_bet_alternative = stack.pop()

        if index == 4:
            bet_alternatives.append(current_bet_alternative)
        else:
            for option in ['1', 'X', '2']:
                if not (current_bet_alternative.count('1') >= 2 and option == '1') and not (index == 3 and option == 'X'):
                    stack.append((index + 1, current_bet_alternative + [option]))

    return bet_alternatives


def generate_pronosport_bet_alternatives_recursive(index, current_bet_alternative, bet_alternatives):
    """
    Generates all bet alternatives in the recursive way using BACKTRACKING
    :param index: Position in the current alternative
    :param current_bet_alternative: the one to-be-generated
    :param bet_alternatives: all bet alternatives, in a list
    :return: the alternatives list
    """
    if index == 4:
        bet_alternatives.append(current_bet_alternative)
    else:
        for option in ['1', 'X', '2']:
            if not (current_bet_alternative.count('1') >= 2 and option == '1') and not (index == 3 and option == 'X'):
                generate_pronosport_bet_alternatives_recursive(index + 1, current_bet_alternative + [option], bet_alternatives)


def iterative_backtracking_generated_pronosport_bets():
    bet_alternatives_iterative = generate_pronosport_bet_alternatives_iterative()
    bet_alternatives_iterative.reverse()
    for i in bet_alternatives_iterative:
        print("|".join(i))


def recursive_backtracking_generated_pronosport_bets():
    bet_alternatives_recursive = []
    generate_pronosport_bet_alternatives_recursive(0, [], bet_alternatives_recursive)
    for i in bet_alternatives_recursive:
        print("/".join(i))


def main_menu():
    print("Hello! Please choose a method for generating PRONOSPORT bet alternatives:")
    print("1. Iterative backtracking!")
    print("2. Recursive backtracking!")
    print("0. Exit program")
    while True:
        input_value = input(">")
        if input_value == '1':
            iterative_backtracking_generated_pronosport_bets()
        if input_value == '2':
            recursive_backtracking_generated_pronosport_bets()
        if input_value == '0':
            print("Bye!")
            break


main_menu()
