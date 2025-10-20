import argparse
import json
import sys
from dataclasses import dataclass
from pathlib import Path
import re
import ctypes
from typing import List, Optional, Tuple

ST_IMPL_BST = 0
ST_IMPL_HASH = 1


@dataclass
class PIFEntry:
    lexeme: str
    token_code: int
    st_index: int


@dataclass
class LexicalError:
    line: int
    column: int
    lexeme: str
    message: str


class SymbolTableLibrary:
    def __init__(self, library_path: Optional[Path] = None):
        self.library_path = self._resolve_library_path(library_path)
        self.lib = ctypes.CDLL(str(self.library_path))
        self._configure_functions()

    @staticmethod
    def _resolve_library_path(library_path: Optional[Path]) -> Path:
        if library_path:
            return library_path
        base_dir = Path(__file__).resolve().parent.parent.parent
        build_dir = base_dir / "build"
        candidates: List[Path] = []
        if sys.platform == "darwin":
            candidates.append(build_dir / "libsymbol_table.dylib")
        elif sys.platform.startswith("linux"):
            candidates.append(build_dir / "libsymbol_table.so")
        elif sys.platform == "win32":
            candidates.append(build_dir / "symbol_table.dll")
        else:
            raise RuntimeError(f"Unsupported platform: {sys.platform}")
        for candidate in candidates:
            if candidate.exists():
                return candidate
        raise FileNotFoundError(
            "Symbol table library not found. Build it with `make` inside src/c/."
        )

    def _configure_functions(self) -> None:
        c_void_p = ctypes.c_void_p
        c_char_p = ctypes.c_char_p
        c_int = ctypes.c_int
        c_size_t = ctypes.c_size_t

        self.lib.st_create.argtypes = [c_int, c_size_t]
        self.lib.st_create.restype = c_void_p

        self.lib.st_free.argtypes = [c_void_p]
        self.lib.st_free.restype = None

        self.lib.st_insert.argtypes = [c_void_p, c_char_p]
        self.lib.st_insert.restype = c_int

        self.lib.st_get_id.argtypes = [c_void_p, c_char_p]
        self.lib.st_get_id.restype = c_int

        self.lib.st_size.argtypes = [c_void_p]
        self.lib.st_size.restype = c_size_t

        self.lib.st_symbol_at.argtypes = [c_void_p, c_size_t]
        self.lib.st_symbol_at.restype = c_char_p


class SymbolTable:
    def __init__(self, lib: SymbolTableLibrary, impl: int, capacity: int = 0):
        self._lib = lib
        self._table = self._lib.lib.st_create(impl, capacity)
        if not self._table:
            raise RuntimeError("Failed to create symbol table")
        self._closed = False

    def insert(self, symbol: str) -> int:
        encoded = symbol.encode("utf-8")
        result = self._lib.lib.st_insert(self._table, ctypes.c_char_p(encoded))
        if result == -1:
            raise RuntimeError(f"Failed to insert symbol `{symbol}` into symbol table")
        return result

    def get_id(self, symbol: str) -> int:
        encoded = symbol.encode("utf-8")
        return self._lib.lib.st_get_id(self._table, ctypes.c_char_p(encoded))

    def values(self) -> List[str]:
        size = self._lib.lib.st_size(self._table)
        entries: List[str] = []
        for index in range(size):
            value = self._lib.lib.st_symbol_at(self._table, index)
            if value is None:
                entries.append("")
            else:
                entries.append(value.decode("utf-8"))
        return entries

    def close(self) -> None:
        if not self._closed:
            self._lib.lib.st_free(self._table)
            self._closed = True

    def __del__(self) -> None:
        self.close()


class LexicalAnalyzer:
    KEYWORDS = {
        "on",
        "when",
        "send_command",
        "func",
        "for",
        "in",
        "if",
        "end",
    }

    TOKEN_PATTERNS: List[Tuple[str, re.Pattern[str]]] = [
        ("ARROW", re.compile(r"^->")),
        ("RELOP", re.compile(r"^(==|!=|>=|<=|>|<)")),
        ("STRING", re.compile(r'^"[A-Za-z0-9 _\-\.,:]*"')),
        ("NUMBER", re.compile(r"^\d+")),
        ("IDENTIFIER", re.compile(r"^[A-Za-z][A-Za-z0-9_]*")),
        ("LBRACKET", re.compile(r"^\[")),
        ("RBRACKET", re.compile(r"^\]")),
        ("LPAREN", re.compile(r"^\(")),
        ("RPAREN", re.compile(r"^\)")),
        ("COLON", re.compile(r"^:")),
        ("COMMA", re.compile(r"^,")),
        ("PLUS", re.compile(r"^\+")),
        ("MINUS", re.compile(r"^-")),
        ("STAR", re.compile(r"^\*")),
        ("SLASH", re.compile(r"^/")),
        ("PERCENT", re.compile(r"^%")),
        ("ASSIGN", re.compile(r"^=")),
    ]

    def __init__(self, token_codes: dict, lib: SymbolTableLibrary):
        self.token_codes = token_codes
        self.lib = lib
        self.identifiers = SymbolTable(self.lib, ST_IMPL_BST)
        self.constants = SymbolTable(self.lib, ST_IMPL_HASH, capacity=97)

    def analyze(self, source: str) -> Tuple[List[PIFEntry], List[LexicalError]]:
        pif_entries: List[PIFEntry] = []
        errors: List[LexicalError] = []

        lines = source.splitlines()
        for line_number, line in enumerate(lines, start=1):
            index = 0
            length = len(line)
            while index < length:
                current_char = line[index]
                if current_char in " \t\r":
                    index += 1
                    continue
                if current_char == '#':
                    break
                segment = line[index:]
                match_lexeme: Optional[str] = None
                token_type: Optional[str] = None
                for candidate_type, pattern in self.TOKEN_PATTERNS:
                    match = pattern.match(segment)
                    if match:
                        match_lexeme = match.group(0)
                        token_type = candidate_type
                        break
                if match_lexeme is None or token_type is None:
                    if current_char == '"':
                        errors.append(
                            LexicalError(
                                line_number,
                                index + 1,
                                segment,
                                "Unterminated string literal",
                            )
                        )
                        break
                    errors.append(
                        LexicalError(
                            line_number,
                            index + 1,
                            current_char,
                            "Unrecognized token",
                        )
                    )
                    index += 1
                    continue

                token_code, st_index = self._record_token(token_type, match_lexeme)
                pif_entries.append(PIFEntry(match_lexeme, token_code, st_index))
                index += len(match_lexeme)

        return pif_entries, errors

    def _record_token(self, token_type: str, lexeme: str) -> Tuple[int, int]:
        if token_type == "IDENTIFIER":
            if lexeme in self.KEYWORDS:
                code = self.token_codes[lexeme]
                return code, -1
            code = self.token_codes["identifier"]
            st_index = self.identifiers.insert(lexeme)
            return code, st_index
        if token_type == "NUMBER":
            code = self.token_codes["integer_constant"]
            st_index = self.constants.insert(lexeme)
            return code, st_index
        if token_type == "STRING":
            code = self.token_codes["string_constant"]
            st_index = self.constants.insert(lexeme)
            return code, st_index

        lexeme_to_code = {
            "ARROW": "->",
            "LBRACKET": "[",
            "RBRACKET": "]",
            "LPAREN": "(",
            "RPAREN": ")",
            "COLON": ":",
            "COMMA": ",",
            "PLUS": "+",
            "MINUS": "-",
            "STAR": "*",
            "SLASH": "/",
            "PERCENT": "%",
            "ASSIGN": "=",
        }
        if token_type == "RELOP":
            code = self.token_codes[lexeme]
            return code, -1
        if token_type in lexeme_to_code:
            code_key = lexeme_to_code[token_type]
            code = self.token_codes[code_key]
            return code, -1

        raise ValueError(f"Unhandled token type `{token_type}` for lexeme `{lexeme}`")

    def dump_identifier_symbols(self) -> List[Tuple[int, str]]:
        return list(enumerate(self.identifiers.values()))

    def dump_constant_symbols(self) -> List[Tuple[int, str]]:
        return list(enumerate(self.constants.values()))

    def close(self) -> None:
        self.identifiers.close()
        self.constants.close()


def load_tokens(path: Path) -> dict:
    with path.open("r", encoding="utf-8") as handle:
        payload = json.load(handle)
    return payload["tokens"]


def write_pif(path: Path, entries: List[PIFEntry]) -> None:
    with path.open("w", encoding="utf-8") as handle:
        handle.write("# token_code st_index lexeme\n")
        for entry in entries:
            handle.write(f"{entry.token_code:>4} {entry.st_index:>6} {entry.lexeme}\n")


def write_symbol_table(path: Path, entries: List[Tuple[int, str]], header: str) -> None:
    with path.open("w", encoding="utf-8") as handle:
        handle.write(f"# {header}\n")
        for idx, symbol in entries:
            handle.write(f"{idx:>4}: {symbol}\n")


def write_errors(path: Path, errors: List[LexicalError]) -> None:
    with path.open("w", encoding="utf-8") as handle:
        if not errors:
            handle.write("No lexical errors detected.\n")
            return
        for error in errors:
            handle.write(
                f"line {error.line}, column {error.column}: {error.message} -> {error.lexeme}\n"
            )


def run_cli() -> None:
    parser = argparse.ArgumentParser(
        description="IoTiny lexical analyzer using the C-backed symbol table"
    )
    parser.add_argument("--source", required=True, help="Path to the source program")
    parser.add_argument(
        "--tokens",
        default=str(
            Path(__file__).resolve().parent.parent.parent / "config" / "tokens.json"
        ),
        help="Path to the token definition JSON file",
    )
    parser.add_argument(
        "--output-dir",
        default=str(Path(__file__).resolve().parent.parent.parent / "output"),
        help="Directory where analysis artifacts will be written",
    )
    parser.add_argument(
        "--prefix",
        default=None,
        help="Optional prefix for output files (defaults to the source stem)",
    )
    parser.add_argument(
        "--lib",
        default=None,
        help="Optional path to the compiled symbol table shared library",
    )

    args = parser.parse_args()

    source_path = Path(args.source).resolve()
    tokens_path = Path(args.tokens).resolve()
    output_dir = Path(args.output_dir).resolve()
    output_dir.mkdir(parents=True, exist_ok=True)

    prefix = args.prefix if args.prefix else source_path.stem
    pif_path = output_dir / f"{prefix}_pif.txt"
    id_st_path = output_dir / f"{prefix}_identifiers_st.txt"
    const_st_path = output_dir / f"{prefix}_constants_st.txt"
    error_path = output_dir / f"{prefix}_lexical_errors.txt"

    lib_path = Path(args.lib).resolve() if args.lib else None

    token_codes = load_tokens(tokens_path)
    library = SymbolTableLibrary(lib_path)

    with source_path.open("r", encoding="utf-8") as src_handle:
        source_code = src_handle.read()

    analyzer = LexicalAnalyzer(token_codes, library)
    try:
        pif_entries, errors = analyzer.analyze(source_code)
        write_pif(pif_path, pif_entries)
        write_symbol_table(
            id_st_path,
            analyzer.dump_identifier_symbols(),
            "Identifier Symbol Table (BST-backed)",
        )
        write_symbol_table(
            const_st_path,
            analyzer.dump_constant_symbols(),
            "Constant Symbol Table (hash-table-backed)",
        )
        write_errors(error_path, errors)
    finally:
        analyzer.close()

    if errors:
        print(
            f"Lexical analysis completed with {len(errors)} error(s). See {error_path}",
            file=sys.stderr,
        )
    else:
        print(f"Lexical analysis completed successfully. Outputs stored under {output_dir}.")


if __name__ == "__main__":
    run_cli()
