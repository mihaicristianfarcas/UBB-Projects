# Lab 3 — IoTiny Lexical Analysis

## 1. Overview

This laboratory extends the IoTiny DSL compiler toolchain with a lexical analysis phase. Three complementary tooling approaches are provided:

- **flex/lex scanner** (`lex/iotiny_lexer.l`) for grammar-aware tokenization demonstrations.
- **Python regex-based analyzer** (`src/python/lexer.py`) that produces the required artifacts: Program Internal Form (PIF), Symbol Tables (ST), and lexical error reports.
- **C symbol table library** (`src/c`) offering both **BST** and **hash-table** implementations that the Python analyzer loads dynamically.

## 2. Token Specification

Token codes are defined in `config/tokens.json`. Categories include:

- **Keywords**: `on`, `when`, `send_command`, `func`, `for`, `in`, `if`, `end`.
- **Separators and punctuation**: `(`, `)`, `[`, `]`, `:`, `,`.
- **Operators**: assignment `=`, arithmetic `+ - * / %`, comparisons `> < == != >= <=`, trigger arrow `->`.
- **Identifiers**: sequences matching `[A-Za-z][A-Za-z0-9_]*`, stored in the identifier ST.
- **Integer constants**: decimal numbers matched by `\d+`, stored in the constant ST.
- **String literals**: double-quoted ASCII payloads, also kept in the constant ST.

Both the flex lexer and the Python analyzer rely on these definitions to keep token classification consistent.

## 3. Symbol Table Management (C)

The shared library exposes a uniform C API (`symbol_table.h`):

- `st_create(impl, capacity)` selects between `ST_IMPL_BST` (binary search tree) and `ST_IMPL_HASH` (chained hash table). The BST is used for identifiers to preserve lexical order, while the hash table provides near-constant lookups for constants.
- `st_insert`, `st_get_id` perform lookup / insert in `O(log n)` (BST) or expected `O(1)` (hash).
- `st_size`, `st_symbol_at` allow enumerating the stored symbols when exporting ST files.

Implementation details:

- **BST**: unbalanced binary tree storing `(key, id)` pairs with recursive insert/search.
- **Hash table**: DJB2 hashing with separate chaining, configurable bucket count (default 31, analyzer uses 97 for constants).
- Dynamic arrays keep an ordered list of inserted symbols so that Python can index by ID without traversing the structures.

## 4. Python Regex Analyzer

`src/python/lexer.py` orchestrates lexical analysis:

1. Loads token codes from JSON.
2. Dynamically links against the C symbol table library (built into `build/libsymbol_table.dylib`).
3. Iterates source files line by line, using compiled regexes to classify the next lexeme.
4. Distinguishes keywords from identifiers, populates the appropriate ST, and appends `(token_code, st_index)` pairs to the PIF.
5. Collects lexical errors with precise line/column metadata; unterminated strings and unknown characters are handled explicitly.
6. Emits artifacts under `output/`:
   - `<prefix>_pif.txt`
   - `<prefix>_identifiers_st.txt`
   - `<prefix>_constants_st.txt`
   - `<prefix>_lexical_errors.txt`

### Running the analyzer

```bash
cd ".../Lab3/src/c"
make  # builds build/libsymbol_table.dylib

cd ".../Lab3"
".venv/bin/python" src/python/lexer.py --source samples/program1.iotiny
".venv/bin/python" src/python/lexer.py --source samples/program2.iotiny
```

Optional flags:

- `--output-dir <path>` to redirect artifacts.
- `--prefix <name>` to override the output file prefix.
- `--lib <path>` to point at a custom-built symbol table library.

## 5. flex/lex Scanner

The flex specification (`lex/iotiny_lexer.l`) mirrors the token definitions and prints token tags to stdout. To build and run it with flex:

```bash
cd ".../Lab3/lex"
flex iotiny_lexer.l
cc lex.yy.c -o iotiny_lexer
./iotiny_lexer < ../samples/program1.iotiny
```

This scanner is primarily for validation and pedagogical comparison with the Python implementation.

## 6. Sample Outputs

Running the analyzer on the provided IoTiny programs yields:

- `output/program1_pif.txt`, `output/program2_pif.txt`: ordered token stream with symbol table indices.
- `output/program1_identifiers_st.txt` / `output/program2_identifiers_st.txt`: identifiers discovered via the BST-backed table.
- `output/program1_constants_st.txt` / `output/program2_constants_st.txt`: constants added through the hash table.
- Both `output/..._lexical_errors.txt` files report _“No lexical errors detected.”_

These artifacts satisfy the laboratory requirements for producing the PIF, ST, and lexical error diagnostics.
