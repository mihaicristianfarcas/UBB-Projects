"""Parse tree builder for LR(0) bottom-up parsing."""

from typing import Dict, List, Optional, Tuple
from common.grammar import Grammar
from common.tree import ParseTree, TreeNode


class LR0ParserWithTree:
    """LR(0) parser that constructs a parse tree."""
    
    def __init__(self, grammar: Grammar, action_table: Dict, goto_table: Dict):
        self.grammar = grammar
        self.action_table = action_table
        self.goto_table = goto_table
    
    def parse(self, input_sequence: List[str]) -> Tuple[List[str], ParseTree]:
        """
        Parse an input sequence and return productions and parse tree.
        
        Args:
            input_sequence: List of terminal symbols (tokens)
        
        Returns:
            Tuple of (list of productions, parse tree)
        """
        # Add end marker
        input_sequence = input_sequence + ['$']
        
        # Initialize parse tree
        tree = ParseTree(self.grammar)
        
        # Initialize stacks - track both states and tree node indices
        state_stack = [0]
        node_stack: List[int] = []
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
                # Shift: push terminal symbol and state
                terminal_node = tree.add_node(current_input)
                node_stack.append(terminal_node)
                state_stack.append(action_value)
                input_idx += 1
            
            elif action_type == 'reduce':
                # Reduce by production
                nonterminal, prod_idx = action_value
                production = self.grammar.get_productions_for(nonterminal)[prod_idx]
                
                # Record production
                prod_str = f"{nonterminal} -> {' '.join(production)}"
                productions.append(prod_str)
                
                # Create nonterminal node
                nt_node = tree.add_node(nonterminal)
                
                # Pop children and link them
                if not (len(production) == 1 and self.grammar.is_epsilon(production[0])):
                    child_nodes = []
                    for _ in production:
                        child_idx = node_stack.pop()
                        state_stack.pop()
                        child_nodes.append(child_idx)
                    
                    # Reverse to get correct order
                    child_nodes.reverse()
                    
                    # Set parent and sibling relationships
                    left_sibling = None
                    for child_idx in child_nodes:
                        child = tree.nodes[child_idx]
                        tree.nodes[child_idx] = TreeNode(
                            index=child.index,
                            symbol=child.symbol,
                            parent=nt_node,
                            left_sibling=left_sibling
                        )
                        left_sibling = child_idx
                else:
                    # Epsilon production
                    epsilon_node = tree.add_node(
                        self.grammar.epsilon,
                        parent=nt_node,
                        left_sibling=None
                    )
                
                # Push nonterminal
                node_stack.append(nt_node)
                
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
                # Accept - the root is the last node on the stack
                return productions, tree
            
            else:
                raise ValueError(f"Unknown action type: {action_type}")
