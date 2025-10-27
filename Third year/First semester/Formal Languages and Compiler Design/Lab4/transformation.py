import collections

def fa_to_rg(fa):
    """
    Transforms a Finite Automaton (FA) into a Regular Grammar (RG).

    The algorithm works as follows:
    1. The non-terminal symbols of the RG are the states of the FA.
    2. The start symbol of the RG is the start state of the FA.
    3. For each transition δ(q_i, a) = q_j in the FA, we add a production rule:
       q_i -> a q_j
    4. If q_j is a final state, we add a second production rule:
       q_i -> a

    Args:
        fa (dict): A dictionary representing the Finite Automaton with the keys:
                   'states', 'alphabet', 'transitions', 'start_state', 'final_states'.

    Returns:
        dict: A dictionary representing the Regular Grammar.
    """
    rg = {
        'non_terminals': fa['states'],
        'terminals': fa['alphabet'],
        'productions': collections.defaultdict(list),
        'start_symbol': fa['start_state']
    }

    for (state_from, symbol), state_to in fa['transitions'].items():
        # Rule 1: Add production q_i -> a q_j
        production_rule = f"{symbol} {state_to}"
        rg['productions'][state_from].append(production_rule)

        # Rule 2: If state_to is a final state, add production q_i -> a
        if state_to in fa['final_states']:
            rg['productions'][state_from].append(symbol)
    
    # Make production rules unique
    for state in rg['productions']:
        rg['productions'][state] = sorted(list(set(rg['productions'][state])))

    return rg

def print_rg(rg):
    """Helper function to print a Regular Grammar in a readable format."""
    print(f"Non-terminals: {', '.join(sorted(list(rg['non_terminals'])))}")
    print(f"Terminals: {', '.join(sorted(list(rg['terminals'])))}")
    print(f"Start Symbol: {rg['start_symbol']}")
    print("Productions:")
    for state_from, prods in sorted(rg['productions'].items()):
        for prod in prods:
            print(f"  {state_from} -> {prod}")

if __name__ == "__main__":
    # --- Example: FA for Identifier ---
    # This FA recognizes identifiers starting with a letter, followed by letters or digits.
    # To keep the example concise, we use a simplified alphabet.
    # L = {a, b}, D = {1, 2}
    # Real-world alphabet would be [A-Za-z] and [A-Za-z0-9_]

    identifier_fa = {
        'states': {'q0', 'q1'},
        'alphabet': {'a', 'b', '1', '2'},
        'transitions': {
            # Transitions from start state q0 on a letter
            ('q0', 'a'): 'q1',
            ('q0', 'b'): 'q1',
            # Transitions from state q1 (loop)
            ('q1', 'a'): 'q1',
            ('q1', 'b'): 'q1',
            ('q1', '1'): 'q1',
            ('q1', '2'): 'q1',
        },
        'start_state': 'q0',
        'final_states': {'q1'}
    }

    print("--- FA to RG Transformation Example ---")
    print("\n--- Input: Finite Automaton for a simplified Identifier ---")
    print(f"States: {identifier_fa['states']}")
    print(f"Alphabet: {identifier_fa['alphabet']}")
    print(f"Start State: {identifier_fa['start_state']}")
    print(f"Final States: {identifier_fa['final_states']}")
    
    # Perform the transformation
    identifier_rg = fa_to_rg(identifier_fa)

    print("\n--- Output: Equivalent Regular Grammar ---")
    print_rg(identifier_rg)
