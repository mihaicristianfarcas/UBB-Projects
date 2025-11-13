#!/usr/bin/env python3
"""
Test suite for Finite Automata and Regular Grammar implementations.
Demonstrates that FA and RG correctly recognize identifiers, numbers, and strings.
"""

import sys
from pathlib import Path

# Add src/python to path
src_path = Path(__file__).resolve().parent / "src" / "python"
sys.path.insert(0, str(src_path))

from finite_automaton import IdentifierFA, NumberFA, StringFA
from regular_grammar import IdentifierRG, NumberRG, StringRG


def test_identifier_fa():
    """Test Finite Automaton for identifiers."""
    print("=" * 60)
    print("Testing Identifier Finite Automaton")
    print("=" * 60)
    
    fa = IdentifierFA()
    
    # Valid identifiers
    valid = ["x", "temperature", "temp_sensor", "value123", "LED_PIN", "myVar_1"]
    
    # Invalid identifiers
    invalid = ["123abc", "_start", "my-var", "123", ""]
    
    print("\nValid identifiers:")
    for test in valid:
        result = fa.accepts(test)
        status = "✓ PASS" if result else "✗ FAIL"
        print(f"  {status}: '{test}' -> {result}")
    
    print("\nInvalid identifiers:")
    for test in invalid:
        result = fa.accepts(test)
        status = "✓ PASS" if not result else "✗ FAIL"
        print(f"  {status}: '{test}' -> {result}")
    
    print()


def test_number_fa():
    """Test Finite Automaton for numbers."""
    print("=" * 60)
    print("Testing Number Finite Automaton")
    print("=" * 60)
    
    fa = NumberFA()
    
    # Valid numbers
    valid = ["0", "1", "42", "123", "999", "1000000"]
    
    # Invalid numbers
    invalid = ["", "12.5", "-5", "+10", "1e5", "0x1F"]
    
    print("\nValid numbers:")
    for test in valid:
        result = fa.accepts(test)
        status = "✓ PASS" if result else "✗ FAIL"
        print(f"  {status}: '{test}' -> {result}")
    
    print("\nInvalid numbers:")
    for test in invalid:
        result = fa.accepts(test)
        status = "✓ PASS" if not result else "✗ FAIL"
        print(f"  {status}: '{test}' -> {result}")
    
    print()


def test_string_fa():
    """Test Finite Automaton for strings."""
    print("=" * 60)
    print("Testing String Finite Automaton")
    print("=" * 60)
    
    fa = StringFA()
    
    # Valid strings
    valid = [
        '""',
        '"hello"',
        '"Temperature: 25C"',
        '"LED_1"',
        '"value-123"',
        '"test.log"',
    ]
    
    # Invalid strings
    invalid = [
        '"',
        'hello',
        '"unterminated',
        '"invalid@char"',
        '"no$special"',
    ]
    
    print("\nValid strings:")
    for test in valid:
        result = fa.accepts(test)
        status = "✓ PASS" if result else "✗ FAIL"
        print(f"  {status}: '{test}' -> {result}")
    
    print("\nInvalid strings:")
    for test in invalid:
        result = fa.accepts(test)
        status = "✓ PASS" if not result else "✗ FAIL"
        print(f"  {status}: '{test}' -> {result}")
    
    print()


def test_identifier_rg():
    """Test Regular Grammar for identifiers."""
    print("=" * 60)
    print("Testing Identifier Regular Grammar")
    print("=" * 60)
    
    rg = IdentifierRG()
    
    # Valid identifiers
    valid = ["x", "temperature", "temp_sensor", "value123"]
    
    # Invalid identifiers
    invalid = ["123abc", "_start", ""]
    
    print("\nValid identifiers:")
    for test in valid:
        result = rg.can_derive(test)
        status = "✓ PASS" if result else "✗ FAIL"
        print(f"  {status}: '{test}' -> {result}")
    
    print("\nInvalid identifiers:")
    for test in invalid:
        result = rg.can_derive(test)
        status = "✓ PASS" if not result else "✗ FAIL"
        print(f"  {status}: '{test}' -> {result}")
    
    print()


def test_number_rg():
    """Test Regular Grammar for numbers."""
    print("=" * 60)
    print("Testing Number Regular Grammar")
    print("=" * 60)
    
    rg = NumberRG()
    
    # Valid numbers
    valid = ["0", "1", "42", "123", "999"]
    
    # Invalid numbers
    invalid = ["", "12.5", "-5"]
    
    print("\nValid numbers:")
    for test in valid:
        result = rg.can_derive(test)
        status = "✓ PASS" if result else "✗ FAIL"
        print(f"  {status}: '{test}' -> {result}")
    
    print("\nInvalid numbers:")
    for test in invalid:
        result = rg.can_derive(test)
        status = "✓ PASS" if not result else "✗ FAIL"
        print(f"  {status}: '{test}' -> {result}")
    
    print()


def test_string_rg():
    """Test Regular Grammar for strings."""
    print("=" * 60)
    print("Testing String Regular Grammar")
    print("=" * 60)
    
    rg = StringRG()
    
    # Valid strings
    valid = ['""', '"hello"', '"LED_1"']
    
    # Invalid strings
    invalid = ['"', 'hello', '"unterminated']
    
    print("\nValid strings:")
    for test in valid:
        result = rg.can_derive(test)
        status = "✓ PASS" if result else "✗ FAIL"
        print(f"  {status}: '{test}' -> {result}")
    
    print("\nInvalid strings:")
    for test in invalid:
        result = rg.can_derive(test)
        status = "✓ PASS" if not result else "✗ FAIL"
        print(f"  {status}: '{test}' -> {result}")
    
    print()


def test_fa_matching():
    """Test FA longest match functionality."""
    print("=" * 60)
    print("Testing FA Longest Match")
    print("=" * 60)
    
    id_fa = IdentifierFA()
    num_fa = NumberFA()
    
    test_cases = [
        ("temperature > 25", id_fa, "temperature"),
        ("123 + 456", num_fa, "123"),
        ("var_1 = 100", id_fa, "var_1"),
    ]
    
    print("\nLongest match tests:")
    for input_str, fa, expected in test_cases:
        result = fa.match(input_str)
        status = "✓ PASS" if result == expected else "✗ FAIL"
        print(f"  {status}: '{input_str}' -> matched '{result}' (expected '{expected}')")
    
    print()


def main():
    """Run all tests."""
    print("\n" + "=" * 60)
    print("FA/RG Test Suite for IoTiny Lexical Analysis")
    print("=" * 60 + "\n")
    
    # Test Finite Automata
    test_identifier_fa()
    test_number_fa()
    test_string_fa()
    
    # Test Regular Grammars
    test_identifier_rg()
    test_number_rg()
    test_string_rg()
    
    # Test matching functionality
    test_fa_matching()
    
    print("=" * 60)
    print("All tests completed!")
    print("=" * 60)


if __name__ == "__main__":
    main()
