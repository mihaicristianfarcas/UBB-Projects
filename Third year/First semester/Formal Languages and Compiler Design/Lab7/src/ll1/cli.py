"""Command-line interface for LL(1) predictive parser."""

import argparse
import sys
from pathlib import Path
from typing import Dict, List

from common.grammar import Grammar
from ll1.predictive_parser import LL1Parser
from common.tree import LL1ParserWithTree
from common.io_utils import (
    load_grammar,
    load_tokens,
    load_pif,
    save_productions,
    save_derivations,
    save_parse_tree_table,
)


def print_first_follow_sets(parser: LL1Parser) -> None:
    """Print FIRST and FOLLOW sets."""
    print("\n=== FIRST Sets ===")
    for symbol in sorted(parser.first_sets.keys()):
        if parser.grammar.is_nonterminal(symbol):
            first_set = parser.first_sets[symbol]
            print(f"FIRST({symbol}) = {{{', '.join(sorted(first_set))}}}")
    
    print("\n=== FOLLOW Sets ===")
    for symbol in sorted(parser.follow_sets.keys()):
        follow_set = parser.follow_sets[symbol]
        print(f"FOLLOW({symbol}) = {{{', '.join(sorted(follow_set))}}}")


def print_parsing_table(parser: LL1Parser) -> None:
    """Print the parsing table."""
    print("\n=== Parsing Table ===")
    print(f"{'Nonterminal':<20} {'Terminal':<20} {'Production':<40}")
    print("-" * 80)
    
    for (nonterminal, terminal), (nt, prod_idx) in sorted(parser.parsing_table.items()):
        production = parser.grammar.get_productions_for(nt)[prod_idx]
        prod_str = f"{nt} -> {' '.join(production)}"
        print(f"{nonterminal:<20} {terminal:<20} {prod_str:<40}")


def parse_sequence(
    grammar: Grammar,
    parser: LL1Parser,
    sequence: List[str],
    output_dir: Path,
    prefix: str
) -> None:
    """Parse a sequence and generate outputs."""
    print(f"\n=== Parsing Sequence ===")
    print(f"Input: {' '.join(sequence)}")
    
    try:
        # Create parser with tree
        tree_parser = LL1ParserWithTree(grammar, parser.get_parsing_table())
        productions, tree = tree_parser.parse(sequence)
        
        print(f"\n✓ ACCEPT")
        print(f"Number of productions: {len(productions)}")
        
        # Save outputs
        output_dir.mkdir(parents=True, exist_ok=True)
        
        # Save productions
        productions_path = output_dir / f"{prefix}_productions.txt"
        save_productions(productions_path, productions)
        print(f"Saved productions to: {productions_path}")
        
        # Save derivations
        derivations_path = output_dir / f"{prefix}_derivations.txt"
        save_derivations(derivations_path, productions, grammar)
        print(f"Saved derivations to: {derivations_path}")
        
        # Save parse tree table
        tree_path = output_dir / f"{prefix}_parse_tree.txt"
        save_parse_tree_table(tree_path, tree.to_string())
        print(f"Saved parse tree to: {tree_path}")
        
        # Print first few productions
        print(f"\nFirst 5 productions:")
        for i, prod in enumerate(productions[:5], 1):
            print(f"  {i}. {prod}")
        
        if len(productions) > 5:
            print(f"  ... ({len(productions) - 5} more)")
        
    except ValueError as e:
        print(f"\n✗ REJECT: {e}")
        sys.exit(1)


def run_test_mode(grammar: Grammar, parser: LL1Parser) -> None:
    """Run in test mode with a simple sequence."""
    print("\n=== Test Mode ===")
    
    # Create a simple test sequence based on grammar
    if grammar.start_symbol == "S":
        # Simple test grammar: S -> a A, A -> + a A | epsilon
        test_sequence = ["a", "+", "a", "+", "a"]
    else:
        # For IoTiny grammar, use a minimal valid program
        test_sequence = ["identifier", "=", "integer_constant"]
    
    print(f"Test sequence: {' '.join(test_sequence)}")
    
    try:
        productions = parser.parse(test_sequence)
        print(f"\n✓ ACCEPT")
        print(f"Number of productions: {len(productions)}")
        print("\nProductions:")
        for i, prod in enumerate(productions, 1):
            print(f"  {i}. {prod}")
    except ValueError as e:
        print(f"\n✗ REJECT: {e}")


def main() -> None:
    """Main entry point."""
    parser_args = argparse.ArgumentParser(
        description="LL(1) Parser with parse tree generation"
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
        help="Show FIRST/FOLLOW sets and parsing table"
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
    
    # Create LL(1) parser
    print("\nConstructing LL(1) parser...")
    try:
        ll1_parser = LL1Parser(grammar)
        print("✓ Grammar is LL(1)")
    except ValueError as e:
        print(f"✗ Error: {e}")
        sys.exit(1)
    
    # Show tables if requested
    if args.show_tables:
        print_first_follow_sets(ll1_parser)
        print_parsing_table(ll1_parser)
    
    # Test mode
    if args.test:
        run_test_mode(grammar, ll1_parser)
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
        
        parse_sequence(grammar, ll1_parser, sequence, args.output_dir, args.prefix)
    
    elif args.sequence:
        sequence = args.sequence.split()
        parse_sequence(grammar, ll1_parser, sequence, args.output_dir, args.prefix)
    
    else:
        print("\nNo input provided. Use --pif, --sequence, or --test")
        print("Run with --help for usage information")


if __name__ == "__main__":
    main()
