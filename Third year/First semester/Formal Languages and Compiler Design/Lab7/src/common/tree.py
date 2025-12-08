"""Parse tree representation with father-sibling relation table."""

from dataclasses import dataclass
from typing import Dict, List, Optional, Set, Tuple
from common.grammar import Grammar


@dataclass
class TreeNode:
    """Node in the parse tree."""
    index: int
    symbol: str
    parent: Optional[int]
    left_sibling: Optional[int]
    
    def __repr__(self) -> str:
        return f"Node({self.index}, '{self.symbol}', parent={self.parent}, sibling={self.left_sibling})"


class ParseTree:
    """
    Parse tree representation using father-sibling relation.
    
    The tree is represented as a table where each node has:
    - index: unique node identifier
    - symbol: the grammar symbol (terminal or nonterminal)
    - parent: index of parent node (None for root)
    - left_sibling: index of left sibling (None if no left sibling)
    """
    
    def __init__(self, grammar: Grammar):
        self.grammar = grammar
        self.nodes: List[TreeNode] = []
        self.node_counter = 0
    
    def add_node(self, symbol: str, parent: Optional[int] = None, 
                 left_sibling: Optional[int] = None) -> int:
        """Add a new node to the tree and return its index."""
        node = TreeNode(
            index=self.node_counter,
            symbol=symbol,
            parent=parent,
            left_sibling=left_sibling
        )
        self.nodes.append(node)
        self.node_counter += 1
        return node.index
    
    def get_node(self, index: int) -> TreeNode:
        """Get a node by its index."""
        return self.nodes[index]
    
    def get_table_representation(self) -> List[Tuple[int, str, Optional[int], Optional[int]]]:
        """Get the table representation of the parse tree."""
        return [(node.index, node.symbol, node.parent, node.left_sibling) 
                for node in self.nodes]
    
    def to_string(self) -> str:
        """Generate a string representation of the parse tree table."""
        lines = ["Parse Tree (Father-Sibling Representation):"]
        lines.append(f"{'Index':<8} {'Symbol':<20} {'Parent':<8} {'Left Sibling':<12}")
        lines.append("-" * 60)
        
        for node in self.nodes:
            parent_str = str(node.parent) if node.parent is not None else "-"
            sibling_str = str(node.left_sibling) if node.left_sibling is not None else "-"
            lines.append(f"{node.index:<8} {node.symbol:<20} {parent_str:<8} {sibling_str:<12}")
        
        return "\n".join(lines)


class LL1ParserWithTree:
    """LL(1) parser that constructs a parse tree."""
    
    def __init__(self, grammar: Grammar, parsing_table: Dict[Tuple[str, str], Tuple[str, int]]):
        self.grammar = grammar
        self.parsing_table = parsing_table
    
    def parse(self, input_sequence: List[str]) -> Tuple[List[str], ParseTree]:
        """
        Parse an input sequence and return productions and parse tree.
        
        Args:
            input_sequence: List of terminal symbols (tokens)
        
        Returns:
            Tuple of (list of productions, parse tree)
        """
        # Add end marker
        input_sequence = input_sequence + ["$"]
        
        # Initialize parse tree
        tree = ParseTree(self.grammar)
        root_idx = tree.add_node(self.grammar.start_symbol)
        
        # Initialize stack with (symbol, tree_node_index)
        stack: List[Tuple[str, int]] = [("$", -1), (self.grammar.start_symbol, root_idx)]
        input_idx = 0
        productions = []
        
        while stack:
            top_symbol, top_node_idx = stack[-1]
            current_input = input_sequence[input_idx] if input_idx < len(input_sequence) else "$"
            
            # If top is terminal or end marker
            if self.grammar.is_terminal(top_symbol) or top_symbol == "$":
                if top_symbol == current_input:
                    stack.pop()
                    input_idx += 1
                else:
                    raise ValueError(f"Parse error: expected {top_symbol}, got {current_input}")
            
            # If top is nonterminal
            elif self.grammar.is_nonterminal(top_symbol):
                key = (top_symbol, current_input)
                if key not in self.parsing_table:
                    raise ValueError(
                        f"Parse error: no entry in parsing table for [{top_symbol}, {current_input}]"
                    )
                
                nonterminal, prod_idx = self.parsing_table[key]
                production = self.grammar.get_productions_for(nonterminal)[prod_idx]
                
                # Record production
                prod_str = f"{nonterminal} -> {' '.join(production)}"
                productions.append(prod_str)
                
                # Pop nonterminal
                stack.pop()
                
                # Add children to tree and push to stack in reverse order
                if not (len(production) == 1 and self.grammar.is_epsilon(production[0])):
                    left_sibling_idx = None
                    child_indices = []
                    
                    for symbol in production:
                        child_idx = tree.add_node(
                            symbol=symbol,
                            parent=top_node_idx,
                            left_sibling=left_sibling_idx
                        )
                        child_indices.append((symbol, child_idx))
                        left_sibling_idx = child_idx
                    
                    # Push children to stack in reverse order
                    for symbol, child_idx in reversed(child_indices):
                        stack.append((symbol, child_idx))
                else:
                    # Add epsilon node
                    tree.add_node(
                        symbol=self.grammar.epsilon,
                        parent=top_node_idx,
                        left_sibling=None
                    )
            else:
                raise ValueError(f"Unknown symbol: {top_symbol}")
        
        return productions, tree
