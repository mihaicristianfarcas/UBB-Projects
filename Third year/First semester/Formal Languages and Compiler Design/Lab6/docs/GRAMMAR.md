# IoTiny Grammar

```
program -> statement_list EOF
statement_list -> statement statement_list | epsilon
statement -> listener_definition | trigger_call | send_command_stmt | assignment_or_call | if_statement | for_loop | function_definition
listener_definition -> on string_constant when condition : block end
trigger_call -> string_constant ARROW expression
send_command_stmt -> send_command expression , expression
assignment_or_call -> identifier assignment_or_call_tail
assignment_or_call_tail -> = expression | ( argument_list )
function_definition -> func identifier ( parameter_list ) : block end
parameter_list -> identifier parameter_list_tail | epsilon
parameter_list_tail -> , identifier parameter_list_tail | epsilon
if_statement -> if condition : block end
for_loop -> for identifier in expression : block end
block -> statement_list
condition -> expression relop expression
relop -> > | < | == | != | >= | <=
expression -> term expression_tail
expression_tail -> add_op term expression_tail | epsilon
term -> factor term_tail
term_tail -> mul_op factor term_tail | epsilon
factor -> integer_constant | string_constant | identifier factor_call_tail | list_literal | ( expression )
factor_call_tail -> ( argument_list ) | epsilon
list_literal -> [ list_items ]
list_items -> expression list_items_tail | epsilon
list_items_tail -> , expression list_items_tail | epsilon
argument_list -> expression argument_list_tail | epsilon
argument_list_tail -> , expression argument_list_tail | epsilon
add_op -> + | -
mul_op -> * | / | %
```

Lexer fragments used by ANTLR:

- Keywords: `on`, `when`, `send_command`, `func`, `for`, `in`, `if`, `end`
- Punctuation: `->`, `=`, `:`, `,`, `(`, `)`, `[`, `]`
- Operators: `+`, `-`, `*`, `/`, `%`, `>`, `<`, `==`, `!=`, `>=`, `<=`
- Identifiers: `[a-zA-Z_][a-zA-Z0-9_]*`
- Integer constants: `[0-9]+`
- String constants: double-quoted, supporting escaped characters
- Whitespace (`[ \\t\\r\\n]+`) and comments starting with `#` are skipped by the lexer.
