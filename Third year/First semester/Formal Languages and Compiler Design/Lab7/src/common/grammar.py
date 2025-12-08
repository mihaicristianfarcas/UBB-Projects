"""Grammar representation and utilities."""

from dataclasses import dataclass
from typing import Dict, List, Set


@dataclass
class Grammar:
    """Context-free grammar representation."""
    
    start_symbol: str
    epsilon: str
    terminals: Set[str]
    nonterminals: Set[str]
    productions: Dict[str, List[List[str]]]
    
    def is_terminal(self, symbol: str) -> bool:
        """Check if a symbol is a terminal."""
        return symbol in self.terminals
    
    def is_nonterminal(self, symbol: str) -> bool:
        """Check if a symbol is a nonterminal."""
        return symbol in self.nonterminals
    
    def is_epsilon(self, symbol: str) -> bool:
        """Check if a symbol is epsilon."""
        return symbol == self.epsilon
    
    def get_productions_for(self, nonterminal: str) -> List[List[str]]:
        """Get all productions for a given nonterminal."""
        return self.productions.get(nonterminal, [])
    
    def all_productions(self) -> List[tuple[str, int, List[str]]]:
        """Get all productions as (nonterminal, index, production) tuples."""
        result = []
        for nonterminal, prods in self.productions.items():
            for idx, prod in enumerate(prods):
                result.append((nonterminal, idx, prod))
        return result
