"""LR(0) shift-reduce parser with canonical collection of item sets."""

from dataclasses import dataclass
from typing import Dict, FrozenSet, List, Set, Tuple
from common.grammar import Grammar


@dataclass(frozen=True)
class LR0Item:
    """
    LR(0) item: [A -> α·β]
    
    Represents a production with a dot position indicating how much
    has been recognized.
    """
    nonterminal: str
    production: Tuple[str, ...]
    dot_position: int
    
    def __str__(self) -> str:
        prod_list = list(self.production)
        prod_with_dot = prod_list[:self.dot_position] + ['·'] + prod_list[self.dot_position:]
        return f"[{self.nonterminal} -> {' '.join(prod_with_dot)}]"
    
    def is_complete(self) -> bool:
        """Check if dot is at the end (reduce item)."""
        return self.dot_position >= len(self.production)
    
    def next_symbol(self) -> str | None:
        """Get the symbol after the dot, or None if at end."""
        if self.is_complete():
            return None
        return self.production[self.dot_position]
    
    def advance(self) -> 'LR0Item':
        """Create a new item with dot advanced by one position."""
        return LR0Item(self.nonterminal, self.production, self.dot_position + 1)


@dataclass(frozen=True)
class ItemSet:
    """A set of LR(0) items representing a parser state."""
    items: FrozenSet[LR0Item]
    
    def __str__(self) -> str:
        return '\n'.join(str(item) for item in sorted(self.items, key=str))


class LR0Parser:
    """LR(0) parser with canonical collection and parsing table construction."""
    
    def __init__(self, grammar: Grammar):
        self.grammar = grammar
        self.item_sets: List[ItemSet] = []
        self.goto_table: Dict[Tuple[int, str], int] = {}
        self.action_table: Dict[Tuple[int, str], Tuple[str, int | Tuple[str, int]]] = {}
        
        # Augment grammar with S' -> S
        self.augmented_start = f"{grammar.start_symbol}'"
        
        self._build_canonical_collection()
        self._construct_parsing_table()
    
    def _closure(self, items: Set[LR0Item]) -> Set[LR0Item]:
        """
        Compute closure of a set of items.
        
        If [A -> α·Bβ] is in the set and B is a nonterminal,
        add [B -> ·γ] for all productions B -> γ.
        """
        closure = set(items)
        changed = True
        
        while changed:
            changed = False
            new_items = set()
            
            for item in closure:
                next_sym = item.next_symbol()
                if next_sym and self.grammar.is_nonterminal(next_sym):
                    # Add all productions for this nonterminal
                    for production in self.grammar.get_productions_for(next_sym):
                        # Convert epsilon to empty tuple
                        if len(production) == 1 and self.grammar.is_epsilon(production[0]):
                            prod_tuple = tuple()
                        else:
                            prod_tuple = tuple(production)
                        
                        new_item = LR0Item(next_sym, prod_tuple, 0)
                        if new_item not in closure:
                            new_items.add(new_item)
                            changed = True
            
            closure.update(new_items)
        
        return closure
    
    def _goto(self, items: Set[LR0Item], symbol: str) -> Set[LR0Item]:
        """
        Compute goto(items, symbol).
        
        Returns the set of items obtained by:
        1. Taking items with symbol after the dot
        2. Advancing the dot past the symbol
        3. Computing closure
        """
        moved_items = set()
        
        for item in items:
            if item.next_symbol() == symbol:
                moved_items.add(item.advance())
        
        return self._closure(moved_items)
    
    def _build_canonical_collection(self) -> None:
        """Build the canonical collection of LR(0) item sets."""
        # Start with augmented start item
        start_production = tuple([self.grammar.start_symbol])
        start_item = LR0Item(self.augmented_start, start_production, 0)
        start_set = ItemSet(frozenset(self._closure({start_item})))
        
        self.item_sets = [start_set]
        processed = set()
        to_process = [0]
        
        while to_process:
            state_idx = to_process.pop(0)
            if state_idx in processed:
                continue
            
            processed.add(state_idx)
            current_set = self.item_sets[state_idx]
            
            # Find all symbols that can be shifted
            symbols = set()
            for item in current_set.items:
                next_sym = item.next_symbol()
                if next_sym:
                    symbols.add(next_sym)
            
            # Compute goto for each symbol
            for symbol in symbols:
                goto_items = self._goto(set(current_set.items), symbol)
                if not goto_items:
                    continue
                
                goto_set = ItemSet(frozenset(goto_items))
                
                # Find or add this set
                try:
                    target_idx = self.item_sets.index(goto_set)
                except ValueError:
                    target_idx = len(self.item_sets)
                    self.item_sets.append(goto_set)
                    to_process.append(target_idx)
                
                self.goto_table[(state_idx, symbol)] = target_idx
    
    def _construct_parsing_table(self) -> None:
        """Construct the LR(0) parsing table."""
        conflicts = []
        
        for state_idx, item_set in enumerate(self.item_sets):
            for item in item_set.items:
                if item.is_complete():
                    # Reduce item
                    if item.nonterminal == self.augmented_start:
                        # Accept item
                        key = (state_idx, '$')
                        if key in self.action_table:
                            conflicts.append(f"Conflict at state {state_idx}, $: "
                                           f"{self.action_table[key]} vs accept")
                        self.action_table[key] = ('accept', 0)
                    else:
                        # Reduce by this production
                        prod_idx = self._get_production_index(item.nonterminal, item.production)
                        
                        # Add reduce action for all terminals (including $)
                        for terminal in self.grammar.terminals | {'$'}:
                            key = (state_idx, terminal)
                            action = ('reduce', (item.nonterminal, prod_idx))
                            
                            if key in self.action_table:
                                existing = self.action_table[key]
                                if existing != action:
                                    conflicts.append(
                                        f"Conflict at state {state_idx}, {terminal}: "
                                        f"{existing} vs {action}"
                                    )
                            else:
                                self.action_table[key] = action
                else:
                    # Shift item
                    next_sym = item.next_symbol()
                    if next_sym and self.grammar.is_terminal(next_sym):
                        key = (state_idx, next_sym)
                        target_state = self.goto_table.get(key)
                        if target_state is not None:
                            action = ('shift', target_state)
                            
                            if key in self.action_table:
                                existing = self.action_table[key]
                                if existing != action:
                                    conflicts.append(
                                        f"Conflict at state {state_idx}, {next_sym}: "
                                        f"{existing} vs {action}"
                                    )
                            else:
                                self.action_table[key] = action
        
        if conflicts:
            raise ValueError(f"Grammar is not LR(0). Conflicts:\n" + "\n".join(conflicts))
    
    def _get_production_index(self, nonterminal: str, production: Tuple[str, ...]) -> int:
        """Get the index of a production in the grammar."""
        productions = self.grammar.get_productions_for(nonterminal)
        
        for idx, prod in enumerate(productions):
            # Handle epsilon productions
            if len(production) == 0:
                if len(prod) == 1 and self.grammar.is_epsilon(prod[0]):
                    return idx
            else:
                if tuple(prod) == production:
                    return idx
        
        raise ValueError(f"Production {nonterminal} -> {production} not found")
    
    def parse(self, input_sequence: List[str]) -> List[str]:
        """
        Parse an input sequence using LR(0) parsing.
        
        Args:
            input_sequence: List of terminal symbols (tokens)
        
        Returns:
            List of productions used in rightmost derivation (reversed)
        """
        # Add end marker
        input_sequence = input_sequence + ['$']
        
        # Initialize stacks
        state_stack = [0]
        symbol_stack = []
        input_idx = 0
        productions = []
        
        while True:
            current_state = state_stack[-1]
            current_input = input_sequence[input_idx]
            
            key = (current_state, current_input)
            if key not in self.action_table:
                raise ValueError(
                    f"Parse error: no action for state {current_state}, "
                    f"symbol {current_input}"
                )
            
            action_type, action_value = self.action_table[key]
            
            if action_type == 'shift':
                # Shift: push symbol and state
                symbol_stack.append(current_input)
                state_stack.append(action_value)
                input_idx += 1
            
            elif action_type == 'reduce':
                # Reduce by production
                nonterminal, prod_idx = action_value
                production = self.grammar.get_productions_for(nonterminal)[prod_idx]
                
                # Record production
                prod_str = f"{nonterminal} -> {' '.join(production)}"
                productions.append(prod_str)
                
                # Pop symbols (unless epsilon production)
                if not (len(production) == 1 and self.grammar.is_epsilon(production[0])):
                    for _ in production:
                        symbol_stack.pop()
                        state_stack.pop()
                
                # Push nonterminal
                symbol_stack.append(nonterminal)
                
                # Goto
                goto_state = state_stack[-1]
                goto_key = (goto_state, nonterminal)
                if goto_key not in self.goto_table:
                    raise ValueError(
                        f"Parse error: no goto for state {goto_state}, "
                        f"nonterminal {nonterminal}"
                    )
                
                state_stack.append(self.goto_table[goto_key])
            
            elif action_type == 'accept':
                # Accept
                return productions
            
            else:
                raise ValueError(f"Unknown action type: {action_type}")
    
    def get_item_sets(self) -> List[ItemSet]:
        """Get the canonical collection of item sets."""
        return self.item_sets.copy()
    
    def get_action_table(self) -> Dict[Tuple[int, str], Tuple[str, int | Tuple[str, int]]]:
        """Get the action table."""
        return self.action_table.copy()
    
    def get_goto_table(self) -> Dict[Tuple[int, str], int]:
        """Get the goto table."""
        return self.goto_table.copy()
