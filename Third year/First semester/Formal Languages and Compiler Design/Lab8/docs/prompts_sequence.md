# Lab 8 - GenAI Translation Prompts Sequence

## Objective
Translate the IoTiny LL(1) parser from Python (Lab 7) to C, validate the generated code, and compare with yacc/ANTLR implementation.

## Phase 1: Understanding the Python Implementation

### Prompt 1.1: Analyze Parser Structure
```
Analyze the following Python LL(1) parser implementation and identify its key components:
- Grammar representation (terminals, nonterminals, productions)
- FIRST set computation algorithm
- FOLLOW set computation algorithm
- Parsing table construction
- Parse function with stack-based parsing

[Include: lab7/src/ll1/predictive_parser.py]
[Include: lab7/src/common/grammar.py]

Provide a detailed breakdown of the algorithms used.
```

### Prompt 1.2: Identify Data Structures
```
List all data structures used in this LL(1) parser:
1. Grammar class members
2. Sets (FIRST, FOLLOW)
3. Parsing table (dictionary with tuple keys)
4. Stack for parsing
5. Input sequence handling

For each, suggest an equivalent C data structure.
```

## Phase 2: C Translation Strategy

### Prompt 2.1: Design C Header Files
```
Design C header files for an LL(1) parser with these requirements:
1. grammar.h - Grammar structure with dynamic arrays for productions
2. parser.h - Parser structure with FIRST/FOLLOW sets and parsing table
3. Use standard C data structures (arrays, linked lists, hash tables)
4. Memory management functions (create, destroy, free)

Provide complete header files with:
- Struct definitions
- Function prototypes
- Documentation comments
```

### Prompt 2.2: Implement Grammar Module
```
Translate the Grammar class from Python to C:

Python code:
[Include Grammar class from lab7/src/common/grammar.py]

Requirements:
1. Use struct for Grammar
2. Implement dynamic arrays for productions
3. Create helper functions: is_terminal, is_nonterminal, is_epsilon
4. Implement get_productions_for
5. Add memory management (create_grammar, free_grammar)

Generate: grammar.h and grammar.c
```

### Prompt 2.3: Implement Set Operations
```
Create a set data structure in C for storing strings (terminals/nonterminals):

Requirements:
1. Dynamic array-based set implementation
2. Functions: create_set, add_to_set, contains, union_sets, copy_set, free_set
3. Efficient string comparison
4. Print function for debugging

Generate: set.h and set.c
```

## Phase 3: Core Parser Translation

### Prompt 3.1: Translate FIRST Set Computation
```
Translate this Python FIRST set computation to C:

[Include _compute_first_sets and _first_of_sequence from predictive_parser.py]

Requirements:
1. Use the Set data structure created earlier
2. Implement iterative algorithm with change detection
3. Handle epsilon productions correctly
4. Return dictionary-like structure (hash table or array)

Generate C function: void compute_first_sets(Parser* parser)
```

### Prompt 3.2: Translate FOLLOW Set Computation
```
Translate this Python FOLLOW set computation to C:

[Include _compute_follow_sets from predictive_parser.py]

Requirements:
1. Use Set data structure
2. Add $ to FOLLOW of start symbol
3. Iterate until no changes
4. Handle epsilon in FIRST(beta) case

Generate C function: void compute_follow_sets(Parser* parser)
```

### Prompt 3.3: Translate Parsing Table Construction
```
Translate parsing table construction from Python to C:

[Include _construct_parsing_table from predictive_parser.py]

Requirements:
1. Create 2D table or hash table structure
2. Detect LL(1) conflicts
3. Handle epsilon productions in FIRST
4. Add FOLLOW entries when needed

Generate C function: void construct_parsing_table(Parser* parser)
```

### Prompt 3.4: Translate Parse Function
```
Translate the main parse function from Python to C:

[Include parse() from predictive_parser.py]

Requirements:
1. Stack-based parsing with string stack
2. Input sequence with $ end marker
3. Match terminals, expand nonterminals
4. Record production strings
5. Error handling

Generate C function: char** parse(Parser* parser, char** input, int input_len, int* prod_count)
```

## Phase 4: I/O and Integration

### Prompt 4.1: JSON Grammar Parser
```
Create a C function to read grammar from JSON format:

Input format (from lab7/config/grammar.json):
{
  "start_symbol": "program",
  "epsilon": "epsilon",
  "terminals": ["identifier", "=", "if", ...],
  "nonterminals": ["program", "statement_list", ...],
  "productions": {
    "program": [["statement_list"]],
    ...
  }
}

Requirements:
1. Use a JSON library (cJSON recommended)
2. Parse all grammar components
3. Build Grammar structure
4. Error handling for malformed JSON

Generate: json_parser.h and json_parser.c
```

### Prompt 4.2: Main Program
```
Create main.c that:
1. Reads grammar from JSON file
2. Reads token sequence from PIF file
3. Creates LL(1) parser
4. Computes FIRST/FOLLOW sets
5. Builds parsing table
6. Parses input
7. Outputs:
   - Productions used
   - Parse tree (father-sibling representation)
8. Frees all memory

Include command-line argument handling:
./ll1_parser --grammar <file> --pif <file> --output <dir>
```

## Phase 5: Output Generation

### Prompt 5.1: Parse Tree Builder
```
Translate parse tree generation to C:

[Include tree.py from lab7/src/common/tree.py]

Requirements:
1. Node structure with parent and sibling pointers
2. Build tree during parsing
3. Generate father-sibling table
4. Output in same format as Python version

Generate: tree.h and tree.c
```

### Prompt 5.2: Output Formatter
```
Create output functions:
1. print_productions() - List of productions used
2. print_derivations() - Step-by-step derivation
3. print_parse_tree() - Father-sibling table format

Match Python output format exactly for comparison.
```

## Phase 6: Build System

### Prompt 6.1: Makefile
```
Create a Makefile for the C parser:

Targets:
- all: Build ll1_parser executable
- clean: Remove object files and executable
- test: Run test cases
- compare: Compare with Python implementation

Dependencies:
- cJSON library for JSON parsing
- All .c files compile to .o
- Link into ll1_parser

Include compiler flags: -Wall -Wextra -O2 -g
```

## Summary of Deliverables

From these prompts, we should generate:

### C Source Files
1. grammar.h, grammar.c - Grammar representation
2. set.h, set.c - Set data structure
3. parser.h, parser.c - LL(1) parser implementation
4. tree.h, tree.c - Parse tree builder
5. json_parser.h, json_parser.c - JSON grammar loader
6. io_utils.h, io_utils.c - Input/output utilities
7. main.c - Main program

### Build Files
8. Makefile - Build system

### Documentation
9. This file - Prompt sequence
10. README.md - Usage instructions
11. validation_report.md - Bug reports and fixes
12. comparison.md - Differences with yacc/ANTLR

## Notes

- Each prompt builds on previous ones
- Test incrementally after each module
- Compare output with Python version at each stage
- Document any bugs or differences encountered
- Use valgrind for memory leak detection
