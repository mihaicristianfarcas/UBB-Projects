"""Utilities for loading grammar and token files."""

import json
from pathlib import Path
from typing import Dict, List
from common.grammar import Grammar


def load_grammar(path: Path) -> Grammar:
    """Load grammar from JSON file."""
    with path.open("r", encoding="utf-8") as f:
        data = json.load(f)
    
    return Grammar(
        start_symbol=data["start_symbol"],
        epsilon=data["epsilon"],
        terminals=set(data["terminals"]),
        nonterminals=set(data["nonterminals"]),
        productions=data["productions"]
    )


def load_tokens(path: Path) -> Dict[str, int]:
    """Load token codes from JSON file."""
    with path.open("r", encoding="utf-8") as f:
        data = json.load(f)
    return data["tokens"]


def load_pif(path: Path, code_to_token: Dict[int, str]) -> List[str]:
    """
    Load PIF file and return sequence of token names.
    
    Args:
        path: Path to PIF file
        code_to_token: Mapping from token code to token name
    
    Returns:
        List of token names (terminals)
    """
    tokens = []
    with path.open("r", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            
            parts = line.split(maxsplit=2)
            if len(parts) < 2:
                continue
            
            code = int(parts[0])
            st_index = int(parts[1])
            lexeme = parts[2] if len(parts) == 3 else ""
            
            # Map code to token name
            if code in code_to_token:
                token_name = code_to_token[code]
                tokens.append(token_name)
            else:
                raise ValueError(f"Unknown token code: {code}")
    
    return tokens


def save_productions(path: Path, productions: List[str]) -> None:
    """Save productions to file."""
    with path.open("w", encoding="utf-8") as f:
        f.write("Productions:\n")
        for i, prod in enumerate(productions, 1):
            f.write(f"{i}. {prod}\n")


def save_derivations(path: Path, productions: List[str], grammar: Grammar) -> None:
    """
    Save derivations to file.
    
    Derivations show the step-by-step leftmost derivation.
    """
    with path.open("w", encoding="utf-8") as f:
        f.write("Derivations (Leftmost):\n")
        
        # Start with start symbol
        current = [grammar.start_symbol]
        f.write(f"0. {' '.join(current)}\n")
        
        for i, prod in enumerate(productions, 1):
            # Parse production
            parts = prod.split(" -> ")
            if len(parts) != 2:
                continue
            
            nonterminal = parts[0]
            rhs = parts[1].split()
            
            # Find leftmost nonterminal and replace it
            for j, symbol in enumerate(current):
                if symbol == nonterminal:
                    # Replace with RHS
                    if len(rhs) == 1 and rhs[0] == grammar.epsilon:
                        current = current[:j] + current[j+1:]
                    else:
                        current = current[:j] + rhs + current[j+1:]
                    break
            
            f.write(f"{i}. {' '.join(current) if current else grammar.epsilon}\n")


def save_parse_tree_table(path: Path, tree_str: str) -> None:
    """Save parse tree table to file."""
    with path.open("w", encoding="utf-8") as f:
        f.write(tree_str)
