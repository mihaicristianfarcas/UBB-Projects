"""Command-line interface for LR(0) shift-reduce parser."""

import argparse
import sys
from pathlib import Path
from typing import List

from common.grammar import Grammar
from lr0.shift_reduce_parser import LR0Parser
from lr0.tree_builder import LR0ParserWithTree
from common.io_utils import (
    load_grammar,
    load_tokens,
    load_pif,
    save_productions,
    save_derivations,
    save_parse_tree_table,
)


def print_item_sets(parser: LR0Parser) -> None:
    """Print the canonical collection of LR(0) item sets."""
    print("\n=== Canonical Collection of LR(0) Item Sets ===")
    for idx, item_set in enumerate(parser.item_sets):
        print(f"\nState {idx}:")
        print(item_set)


def print_parsing_table(parser: LR0Parser) -> None:
    """Print the LR(0) parsing table."""
    print("\n=== LR(0) Parsing Table ===")
    
    # Action table
    print("\nAction Table:")
    print(f"{'State':<8} {'Symbol':<20} {'Action':<30}")
    print("-" * 60)
    
    for (state, symbol), (action_type, value) in sorted(parser.action_table.items()):
        if action_type == 'shift':
            action_str = f"shift {value}"
        elif action_type == 'reduce':
            nt, prod_idx = value
            production = parser.grammar.get_productions_for(nt)[prod_idx]
            action_str = f"reduce {nt} -> {' '.join(production)}"
        else:
            action_str = action_type
        
        print(f"{state:<8} {symbol:<20} {action_str:<30}")
    
    # Goto table
    print("\nGoto Table:")
    print(f"{'State':<8} {'Symbol':<20} {'Goto State':<12}")
    print("-" * 42)
    
    for (state, symbol), target in sorted(parser.goto_table.items()):
        if parser.grammar.is_nonterminal(symbol):
            print(f"{state:<8} {symbol:<20} {target:<12}")


def parse_sequence(
    grammar: Grammar,
    parser: LR0Parser,
    sequence: List[str],
    output_dir: Path,
    prefix: str
) -> None:
    """Parse a sequence and generate outputs."""
    print(f"\n=== Parsing Sequence ===")
    print(f"Input: {' '.join(sequence)}")
    
    try:
        # Create parser with tree
        tree_parser = LR0ParserWithTree(
            grammar,
            parser.get_action_table(),
            parser.get_goto_table()
        )
        productions, tree = tree_parser.parse(sequence)
        
        print(f"\n✓ ACCEPT")
        print(f"Number of productions: {len(productions)}")
        
        # Save outputs
        output_dir.mkdir(parents=True, exist_ok=True)
        
        # Save productions
        productions_path = output_dir / f"{prefix}_productions.txt"
        save_productions(productions_path, productions)
        print(f"Saved productions to: {productions_path}")
        
        # Save derivations (rightmost for LR)
        derivations_path = output_dir / f"{prefix}_derivations.txt"
        # For LR, productions are in bottom-up order, reverse for top-down derivation
        save_derivations(derivations_path, list(reversed(productions)), grammar)
        print(f"Saved derivations to: {derivations_path}")
        
        # Save parse tree table
        tree_path = output_dir / f"{prefix}_parse_tree.txt"
        save_parse_tree_table(tree_path, tree.to_string())
        print(f"Saved parse tree to: {tree_path}")
        
        # Print first few productions
        print(f"\nFirst 5 productions (bottom-up order):")
        for i, prod in enumerate(productions[:5], 1):
            print(f"  {i}. {prod}")
        
        if len(productions) > 5:
            print(f"  ... ({len(productions) - 5} more)")
        
    except ValueError as e:
        print(f"\n✗ REJECT: {e}")
        sys.exit(1)


def run_test_mode(grammar: Grammar, parser: LR0Parser) -> None:
    """Run in test mode with a simple sequence."""
    print("\n=== Test Mode ===")
    
    # Create a simple test sequence based on grammar
    if 'a' in grammar.terminals and '+' in grammar.terminals:
        test_sequence = ["a", "+", "a"]
    else:
        # For IoTiny grammar, use a minimal valid program
        test_sequence = ["identifier", "=", "integer_constant"]
    
    print(f"Test sequence: {' '.join(test_sequence)}")
    
    try:
        productions = parser.parse(test_sequence)
        print(f"\n✓ ACCEPT")
        print(f"Number of productions: {len(productions)}")
        print("\nProductions (bottom-up order):")
        for i, prod in enumerate(productions, 1):
            print(f"  {i}. {prod}")
    except ValueError as e:
        print(f"\n✗ REJECT: {e}")


def main() -> None:
    """Main entry point."""
    parser_args = argparse.ArgumentParser(
        description="LR(0) Parser with parse tree generation"
    )
    
    parser_args.add_argument(
        "--grammar",
        type=Path,
        required=True,
        help="Path to grammar JSON file"
    )
    
    parser_args.add_argument(
        "--pif",
        type=Path,
        help="Path to PIF (Program Internal Form) file"
    )
    
    parser_args.add_argument(
        "--tokens",
        type=Path,
        help="Path to tokens JSON file (required with --pif)"
    )
    
    parser_args.add_argument(
        "--sequence",
        type=str,
        help="Space-separated sequence of terminals to parse"
    )
    
    parser_args.add_argument(
        "--output-dir",
        type=Path,
        default=Path("output"),
        help="Output directory for results"
    )
    
    parser_args.add_argument(
        "--prefix",
        type=str,
        default="result",
        help="Prefix for output files"
    )
    
    parser_args.add_argument(
        "--show-tables",
        action="store_true",
        help="Show item sets and parsing table"
    )
    
    parser_args.add_argument(
        "--test",
        action="store_true",
        help="Run in test mode with a simple sequence"
    )
    
    args = parser_args.parse_args()
    
    # Load grammar
    print(f"Loading grammar from: {args.grammar}")
    grammar = load_grammar(args.grammar)
    print(f"Grammar loaded: {len(grammar.nonterminals)} nonterminals, "
          f"{len(grammar.terminals)} terminals")
    
    # Create LR(0) parser
    print("\nConstructing LR(0) parser...")
    try:
        lr0_parser = LR0Parser(grammar)
        print(f"✓ Grammar is LR(0)")
        print(f"Generated {len(lr0_parser.item_sets)} states")
    except ValueError as e:
        print(f"✗ Error: {e}")
        sys.exit(1)
    
    # Show tables if requested
    if args.show_tables:
        print_item_sets(lr0_parser)
        print_parsing_table(lr0_parser)
    
    # Test mode
    if args.test:
        run_test_mode(grammar, lr0_parser)
        return
    
    # Parse from PIF or sequence
    if args.pif:
        if not args.tokens:
            print("Error: --tokens is required when using --pif")
            sys.exit(1)
        
        print(f"\nLoading PIF from: {args.pif}")
        tokens = load_tokens(args.tokens)
        code_to_token = {code: name for name, code in tokens.items()}
        sequence = load_pif(args.pif, code_to_token)
        
        parse_sequence(grammar, lr0_parser, sequence, args.output_dir, args.prefix)
    
    elif args.sequence:
        sequence = args.sequence.split()
        parse_sequence(grammar, lr0_parser, sequence, args.output_dir, args.prefix)
    
    else:
        print("\nNo input provided. Use --pif, --sequence, or --test")
        print("Run with --help for usage information")


if __name__ == "__main__":
    main()
