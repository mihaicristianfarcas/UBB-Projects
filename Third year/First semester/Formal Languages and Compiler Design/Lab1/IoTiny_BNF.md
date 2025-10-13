# IoTiny - BNF Grammar

# A lightweight domain-specific language for IoT automation

<program> ::= <statement_list>

<statement_list> ::= <statement> <statement_list> | <statement>

<statement> ::= <listener_definition>
| <trigger_call>
| <send_command>
| <assignment>
| <if_statement>

<listener_definition> ::= "on" <string> "when" <condition> ":" <block> "end"

<trigger_call> ::= <string> "->" <expression>

<send_command> ::= "send_command" <string> "," <string>

<assignment> ::= <identifier> "=" <expression>

<if_statement> ::= "if" <condition> ":" <block> "end"

<block> ::= <statement_list>

<condition> ::= <expression> <relop> <expression>

<expression> ::= <identifier>
| <number>
| <string>
| <arithmetic_expr>

<arithmetic_expr> ::= <expression> <arith_op> <expression>

<relop> ::= ">" | "<" | "==" | "!=" | ">=" | "<="

<arith_op> ::= "+" | "-" | "\*" | "/" | "%"

<identifier> ::= <letter> {<letter_or_digit_or_underscore>}

<letter*or_digit_or_underscore> ::= <letter> | <digit> | "*"

<letter> ::= "a" | ... | "z" | "A" | ... | "Z"

<digit> ::= "0" | ... | "9"

<number> ::= <digit> {<digit>}

<string> ::= '"' {<string_char>} '"'

<string*char> ::= <letter> | <digit> | " " | "*" | "-" | "."
