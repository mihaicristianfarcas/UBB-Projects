import argparse
import json
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, List, Optional

from antlr4 import CommonTokenStream, FileStream, ParseTreeWalker
from antlr4.ListTokenSource import ListTokenSource
from antlr4.Token import CommonToken, Token as AntlrToken
from antlr4.error.ErrorListener import ErrorListener

ANTLR_GEN_ROOT = Path(__file__).resolve().parents[1] / "antlr_gen"
if str(ANTLR_GEN_ROOT) not in sys.path:
    sys.path.append(str(ANTLR_GEN_ROOT))

from antlr.IoTinyLexer import IoTinyLexer
from antlr.IoTinyListener import IoTinyListener
from antlr.IoTinyParser import IoTinyParser

DEFAULT_TRACE_HEADER = "Productions"


@dataclass
class PifToken:
    type: str
    lexeme: str
    code: int
    st_index: int


class ParseError(Exception):
    """Raised for syntactic errors reported by ANTLR."""


class ThrowingErrorListener(ErrorListener):
    def syntaxError(self, recognizer, offendingSymbol, line, column, msg, e):  # type: ignore[override]
        raise ParseError(f"line {line}:{column} {msg}")


PRODUCTION_BY_CONTEXT: Dict[str, str] = {
    "ProgramRuleContext": "program -> statement_list EOF",
    "StatementListNonEmptyContext": "statement_list -> statement statement_list",
    "StatementListEmptyContext": "statement_list -> epsilon",
    "StatementListenerContext": "statement -> listener_definition",
    "StatementTriggerContext": "statement -> trigger_call",
    "StatementSendCommandContext": "statement -> send_command_stmt",
    "StatementAssignmentOrCallContext": "statement -> assignment_or_call",
    "StatementIfContext": "statement -> if_statement",
    "StatementForContext": "statement -> for_loop",
    "StatementFunctionContext": "statement -> function_definition",
    "ListenerDefinitionRuleContext": "listener_definition -> on string_constant when condition : block end",
    "TriggerCallRuleContext": "trigger_call -> string_constant ARROW expression",
    "SendCommandRuleContext": "send_command_stmt -> send_command expression , expression",
    "AssignmentOrCallRuleContext": "assignment_or_call -> identifier assignment_or_call_tail",
    "AssignmentTailAssignContext": "assignment_or_call_tail -> = expression",
    "AssignmentTailCallContext": "assignment_or_call_tail -> ( argument_list )",
    "FunctionDefinitionRuleContext": "function_definition -> func identifier ( parameter_list ) : block end",
    "ParameterListNonEmptyContext": "parameter_list -> identifier parameter_list_tail",
    "ParameterListEmptyContext": "parameter_list -> epsilon",
    "ParameterListTailNonEmptyContext": "parameter_list_tail -> , identifier parameter_list_tail",
    "ParameterListTailEmptyContext": "parameter_list_tail -> epsilon",
    "IfStatementRuleContext": "if_statement -> if condition : block end",
    "ForLoopRuleContext": "for_loop -> for identifier in expression : block end",
    "BlockRuleContext": "block -> statement_list",
    "ConditionRuleContext": "condition -> expression relop expression",
    "RelopGtContext": "relop -> >",
    "RelopLtContext": "relop -> <",
    "RelopEqContext": "relop -> ==",
    "RelopNeqContext": "relop -> !=",
    "RelopGteContext": "relop -> >=",
    "RelopLteContext": "relop -> <=",
    "ExpressionRuleContext": "expression -> term expression_tail",
    "ExpressionTailNonEmptyContext": "expression_tail -> add_op term expression_tail",
    "ExpressionTailEmptyContext": "expression_tail -> epsilon",
    "TermRuleContext": "term -> factor term_tail",
    "TermTailNonEmptyContext": "term_tail -> mul_op factor term_tail",
    "TermTailEmptyContext": "term_tail -> epsilon",
    "FactorIntegerContext": "factor -> integer_constant",
    "FactorStringContext": "factor -> string_constant",
    "FactorIdentifierContext": "factor -> identifier factor_call_tail",
    "FactorListContext": "factor -> list_literal",
    "FactorParenthesizedContext": "factor -> ( expression )",
    "FactorCallContext": "factor_call_tail -> ( argument_list )",
    "FactorNoCallContext": "factor_call_tail -> epsilon",
    "ListLiteralRuleContext": "list_literal -> [ list_items ]",
    "ListItemsNonEmptyContext": "list_items -> expression list_items_tail",
    "ListItemsEmptyContext": "list_items -> epsilon",
    "ListItemsTailNonEmptyContext": "list_items_tail -> , expression list_items_tail",
    "ListItemsTailEmptyContext": "list_items_tail -> epsilon",
    "ArgumentListNonEmptyContext": "argument_list -> expression argument_list_tail",
    "ArgumentListEmptyContext": "argument_list -> epsilon",
    "ArgumentListTailNonEmptyContext": "argument_list_tail -> , expression argument_list_tail",
    "ArgumentListTailEmptyContext": "argument_list_tail -> epsilon",
    "AddOpPlusContext": "add_op -> +",
    "AddOpMinusContext": "add_op -> -",
    "MulOpMulContext": "mul_op -> *",
    "MulOpDivContext": "mul_op -> /",
    "MulOpModContext": "mul_op -> %",
}


class ProductionTraceListener(IoTinyListener):
    def __init__(self) -> None:
        self.trace: List[str] = []

    def enterEveryRule(self, ctx):  # type: ignore[override]
        name = type(ctx).__name__
        production = PRODUCTION_BY_CONTEXT.get(name)
        if production:
            self.trace.append(production)


def build_token_type_map() -> Dict[str, int]:
    mapping: Dict[str, int] = {}
    for token_type, name in enumerate(IoTinyLexer.symbolicNames):
        if not name or name == "EOF":
            continue
        mapping[name] = token_type
        mapping[name.lower()] = token_type
    for token_type, literal in enumerate(IoTinyLexer.literalNames):
        if not literal or literal == "<INVALID>":
            continue
        value = literal.strip("'")
        mapping[value] = token_type
    mapping.setdefault("integer_constant", IoTinyLexer.INTEGER)
    mapping.setdefault("string_constant", IoTinyLexer.STRING)
    return mapping


TOKEN_TYPE_MAP = build_token_type_map()


def resolve_token_type(token_name: str, lexeme: str) -> int:
    candidates = [token_name, token_name.lower(), token_name.upper(), lexeme, lexeme.lower(), lexeme.upper()]
    for candidate in candidates:
        if candidate in TOKEN_TYPE_MAP:
            return TOKEN_TYPE_MAP[candidate]
    raise ValueError(f"Token `{token_name}` with lexeme `{lexeme}` cannot be mapped to an ANTLR token type")


def load_json(path: Path) -> dict:
    with path.open("r", encoding="utf-8") as handle:
        return json.load(handle)


def load_token_table(path: Path) -> Dict[str, int]:
    data = load_json(path)
    return data["tokens"]


def read_pif(path: Path, code_to_token: Dict[int, str]) -> List[PifToken]:
    tokens: List[PifToken] = []
    with path.open("r", encoding="utf-8") as handle:
        for raw_line in handle:
            line = raw_line.strip()
            if not line or line.startswith("#"):
                continue
            parts = line.split(maxsplit=2)
            if len(parts) < 2:
                raise ValueError(f"Malformed PIF line: `{line}`")
            code = int(parts[0])
            st_index = int(parts[1])
            lexeme = parts[2] if len(parts) == 3 else ""
            token_name = code_to_token.get(code)
            if token_name is None:
                raise ValueError(f"Token code {code} not found in token table")
            tokens.append(PifToken(token_name, lexeme, code, st_index))
    return tokens


def token_stream_from_pif(tokens: List[PifToken]) -> CommonTokenStream:
    antlr_tokens: List[CommonToken] = []
    for entry in tokens:
        token_type = resolve_token_type(entry.type, entry.lexeme)
        common = CommonToken(type=token_type)
        common.text = entry.lexeme if entry.lexeme else entry.type
        antlr_tokens.append(common)
    antlr_tokens.append(CommonToken(type=AntlrToken.EOF))
    return CommonTokenStream(ListTokenSource(antlr_tokens))


def token_stream_from_source(source_path: Path) -> CommonTokenStream:
    input_stream = FileStream(str(source_path), encoding="utf-8")
    lexer = IoTinyLexer(input_stream)
    return CommonTokenStream(lexer)


def run_parser(token_stream: CommonTokenStream) -> List[str]:
    parser = IoTinyParser(token_stream)
    parser.removeErrorListeners()
    parser.addErrorListener(ThrowingErrorListener())
    tree = parser.program()
    listener = ProductionTraceListener()
    ParseTreeWalker().walk(listener, tree)
    return listener.trace


def write_trace(trace: List[str], path: Path) -> None:
    with path.open("w", encoding="utf-8") as handle:
        handle.write("\n".join(trace))


def parse_arguments(repo_root: Path) -> argparse.Namespace:
    default_tokens = repo_root / "lab5" / "config" / "tokens.json"
    parser = argparse.ArgumentParser(description="ANTLR-based parser for the IoTiny DSL")
    source_group = parser.add_mutually_exclusive_group(required=True)
    source_group.add_argument("--pif", type=Path, help="Program Internal Form file to parse")
    source_group.add_argument("--source", type=Path, help="IoTiny source file to parse")
    parser.add_argument(
        "--tokens",
        type=Path,
        default=default_tokens,
        help="Token table JSON (required only when --pif is used)",
    )
    parser.add_argument(
        "--trace-out",
        type=Path,
        help="Optional path to write the production trace",
    )
    return parser.parse_args()


def main() -> None:
    repo_root = Path(__file__).resolve().parents[3]
    args = parse_arguments(repo_root)
    if args.pif:
        if not args.tokens.exists():
            raise FileNotFoundError(
                f"Token table `{args.tokens}` is required for parsing PIF files but was not found"
            )
        token_table = load_token_table(args.tokens)
        code_to_token = {code: name for name, code in token_table.items()}
        pif_tokens = read_pif(args.pif, code_to_token)
        stream = token_stream_from_pif(pif_tokens)
    else:
        if not args.source.exists():
            raise FileNotFoundError(f"Source file `{args.source}` does not exist")
        stream = token_stream_from_source(args.source)
    trace = run_parser(stream)
    print("ACCEPT")
    print(f"{DEFAULT_TRACE_HEADER} (length = {len(trace)}):")
    for index, production in enumerate(trace, start=1):
        print(f"  {index:>2}. {production}")
    if args.trace_out:
        write_trace(trace, args.trace_out)


if __name__ == "__main__":
    try:
        main()
    except ParseError as error:
        print(f"REJECT: {error}", file=sys.stderr)
        sys.exit(1)
    except Exception as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        sys.exit(2)
