# Formal Definitions for Identifiers and Constants

This document provides the formal definitions of Finite Automata (FA) and Regular Grammars (RG) for the identifiers and constants used in the IoTiny language.

---

## 1. Identifier

- **Regular Expression:** `[A-Za-z][A-Za-z0-9_]*`
- **Description:** Starts with a letter, followed by zero or more letters, digits, or underscores.

### Finite Automaton (FA) for Identifier

Let `L` represent any letter `[A-Za-z]` and `A` represent any alphanumeric character or underscore `[A-Za-z0-9_]`.

- **States (Q):** `{ q₀, q₁ }`
  - `q₀`: Start state
  - `q₁`: Final (accepting) state
- **Alphabet (Σ):** The set of all ASCII characters.
- **Start State:** `q₀`
- **Final States (F):** `{ q₁ }`
- **Transitions (δ):**
  - `δ(q₀, L) → q₁`  *(Start with a letter)*
  - `δ(q₁, A) → q₁`  *(Loop on subsequent letters, digits, or underscores)*

### Regular Grammar (RG) for Identifier

- **Non-terminals (N):** `{ S, T }`
- **Terminals (T):** The set of all letters, digits, and the underscore character.
- **Start Symbol:** `S`
- **Productions (P):**
  - `S → l T` for every letter `l ∈ L`
  - `S → l` for every letter `l ∈ L`
  - `T → a T` for every character `a ∈ A`
  - `T → a` for every character `a ∈ A`

---

## 2. Number Constant

- **Regular Expression:** `[0-9]+`
- **Description:** A sequence of one or more digits.

### Finite Automaton (FA) for Number

Let `D` represent any digit `[0-9]`.

- **States (Q):** `{ q₀, q₁ }`
  - `q₀`: Start state
  - `q₁`: Final (accepting) state
- **Alphabet (Σ):** The set of all ASCII characters.
- **Start State:** `q₀`
- **Final States (F):** `{ q₁ }`
- **Transitions (δ):**
  - `δ(q₀, D) → q₁`  *(Start with a digit)*
  - `δ(q₁, D) → q₁`  *(Loop on subsequent digits)*

### Regular Grammar (RG) for Number

- **Non-terminals (N):** `{ S, T }`
- **Terminals (T):** The set of all digits `[0-9]`.
- **Start Symbol:** `S`
- **Productions (P):**
  - `S → d T` for every digit `d ∈ D`
  - `S → d` for every digit `d ∈ D`
  - `T → d T` for every digit `d ∈ D`
  - `T → d` for every digit `d ∈ D`

---

## 3. String Constant

- **Regular Expression:** `"[A-Za-z0-9 _\-\.,:]+"`
- **Description:** A sequence of allowed characters enclosed in double quotes.

### Finite Automaton (FA) for String

Let `C` be the set of allowed characters `[A-Za-z0-9 _\-\.,:]`.

- **States (Q):** `{ q₀, q₁, q₂, q₃ }`
  - `q₀`: Start state
  - `q₁`: State after the opening quote
  - `q₂`: State for content inside the quotes (final state)
  - `q₃`: State after the closing quote (final state)
- **Alphabet (Σ):** The set of all ASCII characters.
- **Start State:** `q₀`
- **Final States (F):** `{ q₃ }`
- **Transitions (δ):**
  - `δ(q₀, ") → q₁`  *(Opening quote)*
  - `δ(q₁, C) → q₂`  *(First character of the string)*
  - `δ(q₂, C) → q₂`  *(Loop on subsequent characters)*
  - `δ(q₂, ") → q₃`  *(Closing quote)*

### Regular Grammar (RG) for String

- **Non-terminals (N):** `{ S, T, U }`
- **Terminals (T):** The set of allowed characters `C` and the double quote `"`.
- **Start Symbol:** `S`
- **Productions (P):**
  - `S → " T`
  - `T → c U` for every character `c ∈ C`
  - `U → c U` for every character `c ∈ C`
  - `U → "`
