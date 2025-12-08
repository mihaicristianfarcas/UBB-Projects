# Lab 7 - Parsing Algorithms Implementation

## Overview

This laboratory implements two fundamental parsing algorithms for context-free grammars:

1. **LL(1) Predictive Parser** - Top-down parser with lookahead
2. **LR(0) Shift-Reduce Parser** - Bottom-up parser with item sets

Both parsers analyze the **IoTiny Domain-Specific Language** grammar and generate parse trees using a **father-sibling representation** (achieving maximum grade of 10/10).

## Why LR(0) Fails on IoTiny Grammar

The LR(0) parser **cannot** handle the IoTiny grammar due to **shift-reduce conflicts**. This is a fundamental limitation of LR(0) parsers:

###  Key Conflicts in IoTiny Grammar

1. **Epsilon Productions**: Productions like `statement_list → epsilon` cause conflicts because LR(0) cannot distinguish when to reduce to epsilon vs. when to shift another statement.

2. **Optional Constructs**: The grammar includes optional elements (like `epsilon` in `expression_tail → + term expression_tail | epsilon`) which create reduce-reduce conflicts.

3. **Lookahead Required**: Many grammar rules require looking ahead one symbol to make correct parsing decisions, but LR(0) has no lookahead capability.

### Example Conflict

```
State 0 contains:
  [program → · statement_list]  (needs to decide: shift or reduce?)
  [statement_list → · epsilon]  (can reduce immediately)
  [statement_list → · statement statement_list]  (can shift)
```

When LR(0) sees terminals like `identifier`, `if`, or `for`, it doesn't know whether to:
- **Shift** (start parsing a statement)
- **Reduce** (use epsilon production for empty statement_list)

This ambiguity creates shift-reduce conflicts that LR(0) cannot resolve.

### Solution: Use More Powerful Parsers

To parse IoTiny with bottom-up parsing, you would need:
- **SLR(1)**: Simple LR with 1 symbol lookahead
- **LALR(1)**: Look-Ahead LR (most commonly used)
- **LR(1)**: Canonical LR with full lookahead

The **LL(1) parser successfully handles IoTiny** because it uses FIRST and FOLLOW sets to make predictive decisions.

## Project Structure

```
lab7/
├── src/
│   ├── common/                    # Shared components
│   │   ├── grammar.py            # Grammar data structure
│   │   ├── tree.py               # Parse tree with father-sibling relation
│   │   └── io_utils.py           # File I/O operations
│   ├── ll1/                      # LL(1) Predictive Parser
│   │   ├── predictive_parser.py  # FIRST/FOLLOW computation & parsing
│   │   └── cli.py                # Command-line interface
│   ├── lr0/                      # LR(0) Shift-Reduce Parser
│   │   ├── shift_reduce_parser.py # Item sets & parsing table
│   │   ├── tree_builder.py       # Bottom-up tree construction
│   │   └── cli.py                # Command-line interface
│   ├── ll1_parser.py             # LL(1) entry point
│   └── lr0_parser.py             # LR(0) entry point
├── config/
│   ├── grammar.json              # IoTiny grammar definition
│   └── tokens.json               # Token code mappings
├── samples/
│   ├── program1.iotiny           # Sample IoTiny program
│   └── program1_pif.txt          # Program Internal Form
├── output/                       # Generated parse trees & derivations
├── documentation.md              # This file
├── README.md                     # Quick start guide
└── test_all.py                   # Comprehensive test suite
```

## Components

### Common Module

**`grammar.py`** - Grammar Representation
- Loads grammar from JSON format
- Stores terminals, nonterminals, and production rules
- Provides helper methods for symbol classification

**`tree.py`** - Parse Tree Structure
- **TreeNode**: Stores symbol, parent index, left sibling index
- **ParseTree**: Manages collection of nodes
- **LL1ParserWithTree**: Builds tree during top-down parsing
- **Father-Sibling Representation**: Efficient tree storage

**`io_utils.py`** - Input/Output Operations
- `load_grammar()`: Load grammar from JSON
- `load_pif()`: Load Program Internal Form
- `save_productions()`: Export production sequence
- `save_derivations()`: Export derivation steps
- `save_parse_tree_table()`: Export tree table

### LL(1) Predictive Parser

**`predictive_parser.py`** - Core Algorithm
- **FIRST Set Computation**: Determines possible first symbols of each nonterminal
- **FOLLOW Set Computation**: Determines symbols that can follow each nonterminal
- **Parsing Table Construction**: Creates M[Nonterminal, Terminal] table
- **Predictive Parsing**: Uses stack-based algorithm with lookahead

**Algorithm**: 
1. Compute FIRST sets for all symbols
2. Compute FOLLOW sets for all nonterminals  
3. Build parsing table M[A, a] for each production
4. Parse input using stack and table lookup

**`cli.py`** - Command-Line Interface
- Argument parsing
- Table display (FIRST/FOLLOW sets, parsing table)
- Result formatting and output generation

### LR(0) Shift-Reduce Parser

**`shift_reduce_parser.py`** - Core Algorithm
- **LR0Item**: Production with dot position [A → α·β]
- **Closure Operation**: Expand items by adding productions
- **Goto Operation**: Move dot past a symbol
- **Canonical Collection**: Build all LR(0) states
- **Action/Goto Tables**: Determine shift/reduce actions
- **Conflict Detection**: Identifies shift-reduce and reduce-reduce conflicts

**Algorithm**:
1. Augment grammar with S' → S
2. Build canonical collection of item sets
3. Construct Action and Goto tables
4. Parse input using shift-reduce algorithm
5. Report conflicts if grammar is not LR(0)

**`tree_builder.py`** - Tree Construction
- Builds parse tree during bottom-up parsing
- Links nodes as reductions occur
- Maintains father-sibling relationships

**`cli.py`** - Command-Line Interface
- Displays item sets and parsing tables
- Shows detailed conflict information
- Generates output files

## Parse Tree Representation

Both parsers use the **father-sibling representation** for maximum grade (10/10):

```
Index    Symbol               Parent   Left Sibling
------------------------------------------------------------
0        program              -        -           
1        statement_list       0        -           
2        statement            1        -           
3        statement_list       1        2           
4        identifier           2        -           
...
```

**Structure**:
- **Index**: Unique node identifier (0, 1, 2, ...)
- **Symbol**: Terminal or nonterminal symbol
- **Parent**: Index of parent node (- for root)
- **Left Sibling**: Index of left sibling (- if first child)

**Benefits**:
- Compact representation
- Easy tree traversal
- Efficient storage
- Clear parent-child relationships

## Usage

### LL(1) Parser

```bash
cd src

# Validate grammar
python3 ll1_parser.py --grammar ../config/grammar.json

# Parse from PIF
python3 ll1_parser.py \
    --grammar ../config/grammar.json \
    --pif ../samples/program1_pif.txt \
    --tokens ../config/tokens.json \
    --output-dir ../output \
    --prefix ll1_iotiny

# Show FIRST/FOLLOW sets
python3 ll1_parser.py --grammar ../config/grammar.json --show-tables
```

### LR(0) Parser

```bash
cd src

# Attempt to build parser (will show conflicts)
python3 lr0_parser.py --grammar ../config/grammar.json

# Show item sets and conflicts
python3 lr0_parser.py --grammar ../config/grammar.json --show-tables
```

### Run Tests

```bash
# Run comprehensive test suite
python3 test_all.py
```

## Grammar Format

Grammars are defined in JSON:

```json
{
  "start_symbol": "program",
  "epsilon": "epsilon",
  "terminals": ["identifier", "=", "if", "end", ...],
  "nonterminals": ["program", "statement", "expression", ...],
  "productions": {
    "program": [["statement_list"]],
    "statement_list": [
      ["statement", "statement_list"],
      ["epsilon"]
    ],
    ...
  }
}
```

## Output Files

Each parsing run generates three files:

1. **`*_productions.txt`** - Sequence of productions used
2. **`*_derivations.txt`** - Step-by-step derivation
3. **`*_parse_tree.txt`** - Father-sibling table representation

## Testing

The test suite verifies:
- ✓ LL(1) validates IoTiny grammar successfully
- ✓ LL(1) parses IoTiny programs correctly
- ✓ LR(0) identifies grammar conflicts as expected
- ✓ Parse trees use correct father-sibling representation
- ✓ All output files are generated

Run with: `python3 test_all.py`

## Implementation Details

### LL(1) Parser Complexity
- FIRST/FOLLOW: O(n³) where n = number of productions
- Parsing: O(m) where m = input length
- Space: O(|N| × |T|) for parsing table

### LR(0) Parser Complexity
- Canonical collection: O(2^n) worst case
- Parsing: O(m) where m = input length
- Space: O(|States| × |Symbols|) for tables

## Why IoTiny is LL(1) but not LR(0)

| Property | LL(1) | LR(0) |
|----------|-------|-------|
| **Lookahead** | 1 symbol | 0 symbols |
| **Direction** | Top-down | Bottom-up |
| **Conflicts** | Resolved by FIRST/FOLLOW | Cannot resolve |
| **Epsilon** | Handles with FOLLOW | Creates conflicts |
| **Power** | Less powerful | More powerful (when no conflicts) |

IoTiny's grammar uses epsilon productions and requires 1-symbol lookahead to disambiguate, which LL(1) provides but LR(0) does not.

## Grading

- **Production string output**: 8.5/10
- **Derivation string output**: 9.0/10
- **Parse tree table (father-sibling)**: **10.0/10** ⭐

This implementation achieves maximum grade with the father-sibling representation.

## References

- Aho, Sethi, Ullman: "Compilers: Principles, Techniques, and Tools"
- Course lectures on LL(1) and LR parsing
- IoTiny DSL specification (Lab 1-3)

---

**Author**: Lab 7 - Formal Languages and Compiler Design  
**Date**: 2025
