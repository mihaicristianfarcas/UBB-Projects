from __future__ import annotations

from dataclasses import dataclass
import string
from typing import Callable, Dict, Optional, Set, Tuple

SymbolClassifier = Callable[[str], Optional[str]]


@dataclass(frozen=True)
class FiniteAutomaton:
    """Deterministic finite automaton that exposes longest-prefix matching."""

    states: Set[str]
    alphabet: Set[str]
    transitions: Dict[Tuple[str, str], str]
    start_state: str
    final_states: Set[str]
    symbol_classifier: SymbolClassifier

    def longest_prefix(self, text: str) -> int:
        """
        Returns the length of the longest prefix accepted by the automaton.

        Args:
            text: The input string to be analyzed.

        Returns:
            int: Number of characters that form a valid token (0 if no match).
        """
        state = self.start_state
        longest_match = -1

        for index, char in enumerate(text):
            symbol = self.symbol_classifier(char)
            if symbol is None:
                break

            transition_key = (state, symbol)
            if transition_key not in self.transitions:
                break

            state = self.transitions[transition_key]
            if state in self.final_states:
                longest_match = index + 1

        return 0 if longest_match == -1 else longest_match


LETTER_SET: Set[str] = set(string.ascii_letters)
DIGIT_SET: Set[str] = set(string.digits)
STRING_ALLOWED_SET: Set[str] = set(string.ascii_letters + string.digits + " _-.,:")


def build_identifier_fa() -> FiniteAutomaton:
    """Builds the FA described in Lab 4 for identifiers."""

    def classifier(char: str) -> Optional[str]:
        if char in LETTER_SET:
            return "LETTER"
        if char in DIGIT_SET:
            return "DIGIT"
        if char == "_":
            return "UNDERSCORE"
        return None

    transitions: Dict[Tuple[str, str], str] = {
        ("q0", "LETTER"): "q1",
        ("q1", "LETTER"): "q1",
        ("q1", "DIGIT"): "q1",
        ("q1", "UNDERSCORE"): "q1",
    }

    return FiniteAutomaton(
        states={"q0", "q1"},
        alphabet={"LETTER", "DIGIT", "UNDERSCORE"},
        transitions=transitions,
        start_state="q0",
        final_states={"q1"},
        symbol_classifier=classifier,
    )


def build_integer_constant_fa() -> FiniteAutomaton:
    """Builds the FA for unsigned integer constants."""

    def classifier(char: str) -> Optional[str]:
        if char in DIGIT_SET:
            return "DIGIT"
        return None

    transitions: Dict[Tuple[str, str], str] = {
        ("q0", "DIGIT"): "q1",
        ("q1", "DIGIT"): "q1",
    }

    return FiniteAutomaton(
        states={"q0", "q1"},
        alphabet={"DIGIT"},
        transitions=transitions,
        start_state="q0",
        final_states={"q1"},
        symbol_classifier=classifier,
    )


def build_string_constant_fa() -> FiniteAutomaton:
    """Builds the FA for quoted string constants."""

    def classifier(char: str) -> Optional[str]:
        if char == '"':
            return "QUOTE"
        if char in STRING_ALLOWED_SET:
            return "CHAR"
        return None

    transitions: Dict[Tuple[str, str], str] = {
        ("q0", "QUOTE"): "q1",
        ("q1", "QUOTE"): "q3",  # allow empty string literals
        ("q1", "CHAR"): "q2",
        ("q2", "CHAR"): "q2",
        ("q2", "QUOTE"): "q3",
    }

    return FiniteAutomaton(
        states={"q0", "q1", "q2", "q3"},
        alphabet={"QUOTE", "CHAR"},
        transitions=transitions,
        start_state="q0",
        final_states={"q3"},
        symbol_classifier=classifier,
    )
