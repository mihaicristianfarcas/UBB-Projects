grammar IoTiny;

program
    : statement_list EOF                             # ProgramRule
    ;

statement_list
    : statement statement_list                       # StatementListNonEmpty
    |                                                # StatementListEmpty
    ;

statement
    : listener_definition                            # StatementListener
    | trigger_call                                   # StatementTrigger
    | send_command_stmt                              # StatementSendCommand
    | assignment_or_call                             # StatementAssignmentOrCall
    | if_statement                                   # StatementIf
    | for_loop                                       # StatementFor
    | function_definition                            # StatementFunction
    ;

listener_definition
    : ON STRING WHEN condition COLON block END       # ListenerDefinitionRule
    ;

trigger_call
    : STRING ARROW expression                        # TriggerCallRule
    ;

send_command_stmt
    : SEND_COMMAND expression COMMA expression       # SendCommandRule
    ;

assignment_or_call
    : IDENTIFIER assignment_or_call_tail             # AssignmentOrCallRule
    ;

assignment_or_call_tail
    : ASSIGN expression                              # AssignmentTailAssign
    | LPAREN argument_list RPAREN                    # AssignmentTailCall
    ;

function_definition
    : FUNC IDENTIFIER LPAREN parameter_list RPAREN COLON block END  # FunctionDefinitionRule
    ;

parameter_list
    : IDENTIFIER parameter_list_tail                 # ParameterListNonEmpty
    |                                                # ParameterListEmpty
    ;

parameter_list_tail
    : COMMA IDENTIFIER parameter_list_tail           # ParameterListTailNonEmpty
    |                                                # ParameterListTailEmpty
    ;

if_statement
    : IF condition COLON block END                   # IfStatementRule
    ;

for_loop
    : FOR IDENTIFIER IN expression COLON block END   # ForLoopRule
    ;

block
    : statement_list                                 # BlockRule
    ;

condition
    : expression relop expression                    # ConditionRule
    ;

relop
    : GT                                             # RelopGt
    | LT                                             # RelopLt
    | EQ                                             # RelopEq
    | NEQ                                            # RelopNeq
    | GTE                                            # RelopGte
    | LTE                                            # RelopLte
    ;

expression
    : term expression_tail                           # ExpressionRule
    ;

expression_tail
    : add_op term expression_tail                    # ExpressionTailNonEmpty
    |                                                # ExpressionTailEmpty
    ;

term
    : factor term_tail                               # TermRule
    ;

term_tail
    : mul_op factor term_tail                        # TermTailNonEmpty
    |                                                # TermTailEmpty
    ;

factor
    : INTEGER                                        # FactorInteger
    | STRING                                         # FactorString
    | IDENTIFIER factor_call_tail                    # FactorIdentifier
    | list_literal                                   # FactorList
    | LPAREN expression RPAREN                       # FactorParenthesized
    ;

factor_call_tail
    : LPAREN argument_list RPAREN                    # FactorCall
    |                                                # FactorNoCall
    ;

list_literal
    : LBRACK list_items RBRACK                       # ListLiteralRule
    ;

list_items
    : expression list_items_tail                     # ListItemsNonEmpty
    |                                                # ListItemsEmpty
    ;

list_items_tail
    : COMMA expression list_items_tail               # ListItemsTailNonEmpty
    |                                                # ListItemsTailEmpty
    ;

argument_list
    : expression argument_list_tail                  # ArgumentListNonEmpty
    |                                                # ArgumentListEmpty
    ;

argument_list_tail
    : COMMA expression argument_list_tail            # ArgumentListTailNonEmpty
    |                                                # ArgumentListTailEmpty
    ;

add_op
    : PLUS                                           # AddOpPlus
    | MINUS                                          # AddOpMinus
    ;

mul_op
    : STAR                                           # MulOpMul
    | DIV                                            # MulOpDiv
    | MOD                                            # MulOpMod
    ;

// Lexer rules
ON            : 'on';
WHEN          : 'when';
SEND_COMMAND  : 'send_command';
FUNC          : 'func';
FOR           : 'for';
IN            : 'in';
IF            : 'if';
END           : 'end';
ARROW         : '->';
ASSIGN        : '=';
COLON         : ':';
COMMA         : ',';
LPAREN        : '(';
RPAREN        : ')';
LBRACK        : '[';
RBRACK        : ']';
PLUS          : '+';
MINUS         : '-';
STAR          : '*';
DIV           : '/';
MOD           : '%';
GT            : '>';
LT            : '<';
EQ            : '==';
NEQ           : '!=';
GTE           : '>=';
LTE           : '<=';

IDENTIFIER    : [a-zA-Z_] [a-zA-Z0-9_]*;
INTEGER       : [0-9]+;
STRING        : '"' ( '\\' . | ~["\\] )* '"';

WS            : [ \t\r\n]+ -> skip;
COMMENT       : '#' ~[\r\n]* -> skip;
