"""LL(1) predictive parser with First and Follow set computation."""

from typing import Dict, List, Set, Tuple
from common.grammar import Grammar


class LL1Parser:
    """LL(1) parser with parsing table construction."""
    
    def __init__(self, grammar: Grammar):
        self.grammar = grammar
        self.first_sets: Dict[str, Set[str]] = {}
        self.follow_sets: Dict[str, Set[str]] = {}
        self.parsing_table: Dict[Tuple[str, str], Tuple[str, int]] = {}
        
        self._compute_first_sets()
        self._compute_follow_sets()
        self._construct_parsing_table()
    
    def _compute_first_sets(self) -> None:
        """Compute FIRST sets for all symbols."""
        # Initialize FIRST sets
        for terminal in self.grammar.terminals:
            self.first_sets[terminal] = {terminal}
        
        for nonterminal in self.grammar.nonterminals:
            self.first_sets[nonterminal] = set()
        
        # Add epsilon to its own FIRST set
        self.first_sets[self.grammar.epsilon] = {self.grammar.epsilon}
        
        # Iterate until no changes
        changed = True
        while changed:
            changed = False
            for nonterminal in self.grammar.nonterminals:
                for production in self.grammar.get_productions_for(nonterminal):
                    old_size = len(self.first_sets[nonterminal])
                    
                    # Compute FIRST of the production
                    first_of_prod = self._first_of_sequence(production)
                    self.first_sets[nonterminal] |= first_of_prod
                    
                    if len(self.first_sets[nonterminal]) > old_size:
                        changed = True
    
    def _first_of_sequence(self, sequence: List[str]) -> Set[str]:
        """Compute FIRST set of a sequence of symbols."""
        if not sequence or (len(sequence) == 1 and self.grammar.is_epsilon(sequence[0])):
            return {self.grammar.epsilon}
        
        result = set()
        all_have_epsilon = True
        
        for symbol in sequence:
            first_of_symbol = self.first_sets.get(symbol, set())
            # Add all except epsilon
            result |= (first_of_symbol - {self.grammar.epsilon})
            
            # If symbol doesn't have epsilon in its FIRST, stop
            if self.grammar.epsilon not in first_of_symbol:
                all_have_epsilon = False
                break
        
        # If all symbols can derive epsilon, add epsilon to result
        if all_have_epsilon:
            result.add(self.grammar.epsilon)
        
        return result
    
    def _compute_follow_sets(self) -> None:
        """Compute FOLLOW sets for all nonterminals."""
        # Initialize FOLLOW sets
        for nonterminal in self.grammar.nonterminals:
            self.follow_sets[nonterminal] = set()
        
        # Add $ to FOLLOW of start symbol
        self.follow_sets[self.grammar.start_symbol].add("$")
        
        # Iterate until no changes
        changed = True
        while changed:
            changed = False
            for nonterminal in self.grammar.nonterminals:
                for production in self.grammar.get_productions_for(nonterminal):
                    # Look at each symbol in the production
                    for i, symbol in enumerate(production):
                        if not self.grammar.is_nonterminal(symbol):
                            continue
                        
                        old_size = len(self.follow_sets[symbol])
                        
                        # Get FIRST of what follows
                        beta = production[i + 1:]
                        first_of_beta = self._first_of_sequence(beta)
                        
                        # Add FIRST(beta) - {epsilon} to FOLLOW(symbol)
                        self.follow_sets[symbol] |= (first_of_beta - {self.grammar.epsilon})
                        
                        # If epsilon is in FIRST(beta), add FOLLOW(nonterminal) to FOLLOW(symbol)
                        if self.grammar.epsilon in first_of_beta:
                            self.follow_sets[symbol] |= self.follow_sets[nonterminal]
                        
                        if len(self.follow_sets[symbol]) > old_size:
                            changed = True
    
    def _construct_parsing_table(self) -> None:
        """Construct the LL(1) parsing table."""
        for nonterminal in self.grammar.nonterminals:
            productions = self.grammar.get_productions_for(nonterminal)
            for prod_idx, production in enumerate(productions):
                first_of_prod = self._first_of_sequence(production)
                
                # For each terminal in FIRST(production), add entry
                for terminal in first_of_prod:
                    if terminal != self.grammar.epsilon:
                        key = (nonterminal, terminal)
                        if key in self.parsing_table:
                            raise ValueError(f"Grammar is not LL(1): conflict at [{nonterminal}, {terminal}]")
                        self.parsing_table[key] = (nonterminal, prod_idx)
                
                # If epsilon is in FIRST(production), add entries for FOLLOW(nonterminal)
                if self.grammar.epsilon in first_of_prod:
                    for terminal in self.follow_sets[nonterminal]:
                        key = (nonterminal, terminal)
                        if key in self.parsing_table:
                            raise ValueError(f"Grammar is not LL(1): conflict at [{nonterminal}, {terminal}]")
                        self.parsing_table[key] = (nonterminal, prod_idx)
    
    def parse(self, input_sequence: List[str]) -> List[str]:
        """
        Parse an input sequence and return the production string.
        
        Args:
            input_sequence: List of terminal symbols (tokens)
        
        Returns:
            List of productions used in the derivation
        """
        # Add end marker
        input_sequence = input_sequence + ["$"]
        
        # Initialize stack with start symbol
        stack = ["$", self.grammar.start_symbol]
        input_idx = 0
        productions = []
        
        while stack:
            top = stack[-1]
            current_input = input_sequence[input_idx] if input_idx < len(input_sequence) else "$"
            
            # If top is terminal
            if self.grammar.is_terminal(top) or top == "$":
                if top == current_input:
                    stack.pop()
                    input_idx += 1
                else:
                    raise ValueError(f"Parse error: expected {top}, got {current_input}")
            
            # If top is nonterminal
            elif self.grammar.is_nonterminal(top):
                key = (top, current_input)
                if key not in self.parsing_table:
                    raise ValueError(f"Parse error: no entry in parsing table for [{top}, {current_input}]")
                
                nonterminal, prod_idx = self.parsing_table[key]
                production = self.grammar.get_productions_for(nonterminal)[prod_idx]
                
                # Record production
                prod_str = f"{nonterminal} -> {' '.join(production)}"
                productions.append(prod_str)
                
                # Pop nonterminal and push production in reverse
                stack.pop()
                if not (len(production) == 1 and self.grammar.is_epsilon(production[0])):
                    for symbol in reversed(production):
                        stack.append(symbol)
            else:
                raise ValueError(f"Unknown symbol: {top}")
        
        return productions
    
    def get_first_sets(self) -> Dict[str, Set[str]]:
        """Get computed FIRST sets."""
        return self.first_sets.copy()
    
    def get_follow_sets(self) -> Dict[str, Set[str]]:
        """Get computed FOLLOW sets."""
        return self.follow_sets.copy()
    
    def get_parsing_table(self) -> Dict[Tuple[str, str], Tuple[str, int]]:
        """Get the parsing table."""
        return self.parsing_table.copy()
