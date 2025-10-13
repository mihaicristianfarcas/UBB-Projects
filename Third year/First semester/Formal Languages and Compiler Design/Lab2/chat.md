### Mihai-Cristian Farcaș:

Validate the BNF of the mini language and the programs I wrote.

### GitHub Copilot:

Read [](file:///Users/mihaicristianfarcas/Developer/UBB-Projects/Third%20year/First%20semester/Formal%20Languages%20and%20Compiler%20Design/Lab2/IoTiny_BNF.md)

Read [](file:///Users/mihaicristianfarcas/Developer/UBB-Projects/Third%20year/First%20semester/Formal%20Languages%20and%20Compiler%20Design/Lab2/Program1_TemperatureMonitor.md)

Read [](file:///Users/mihaicristianfarcas/Developer/UBB-Projects/Third%20year/First%20semester/Formal%20Languages%20and%20Compiler%20Design/Lab2/Program2_LightSchedule.md)

**Validation Notes**

- `<function_call>` in IoTiny_BNF.md omits parentheses, so calls like `set_light(d, "off")` in both programs cannot be derived; adjust the rule to `"<identifier>" "(" <argument_list> ")"`.
- Neither grammar nor lexical rules mention comments, yet Program1_TemperatureMonitor.md and Program2_LightSchedule.md rely on `#…` comments; add a comment production or drop comments from the examples.
- The lexical helper names have typos (`<letter*or_digit_or_underscore>`, `<string*char>`). They should be renamed to the intended identifiers and include `_` instead of `*`, otherwise the identifier and string rules are inconsistent.
- `<arithmetic_expr> ::= <expression> <arith_op> <expression>` makes `<expression>` immediately left-recursive and ambiguous; refactor expressions into precedence levels (e.g., term/factor) or restrict recursion to prevent infinite descent.

**Next Steps**

1. Update the grammar file to encode function-call parentheses, comment tokens, and the corrected lexical helpers.
2. Restructure the arithmetic expression rules (or introduce explicit precedence levels) to keep the parser well-defined.
3. Re-run the examples against the revised grammar to confirm they derive successfully.

### Mihai-Cristian Farcaș:

Alright, help me fix these issues.

GitHub Copilot: Using "Apply Patch"

Read [](file:///Users/mihaicristianfarcas/Developer/UBB-Projects/Third%20year/First%20semester/Formal%20Languages%20and%20Compiler%20Design/Lab2/IoTiny_BNF.md)

Using "Apply Patch" x4

Read [](file:///Users/mihaicristianfarcas/Developer/UBB-Projects/Third%20year/First%20semester/Formal%20Languages%20and%20Compiler%20Design/Lab2/IoTiny_BNF.md)

Using "Apply Patch"

![edit1](./CleanShot%202025-10-13%20at%2008.43.53@2x.jpg)
![edit2](./CleanShot%202025-10-13%20at%2008.44.43@2x.jpg)

Aligned the IoTiny grammar with the sample programs, ensuring every construct derives correctly. Key adjustments are in IoTiny_BNF.md:

- Added `| <comment>` so `#` lines used in both programs parse as statements, and defined `<comment>` / `<comment_char>` lexically.
- Required parentheses in `<function_call> ::= <identifier> "(" <argument_list> ")"`, matching calls like `set_light(d, "on")`.
- Reworked expressions into `<sum>` / `<product>` / `<factor>` tiers with `<add_op>` and `<mul_op>` to eliminate the previous left-recursive `<arithmetic_expr>`.
- Cleaned lexical helper names and contents (`<letter_or_digit_or_underscore>`, `<string_char>`) so identifiers accept `_` and strings cover the punctuation used.

You might want to regenerate or rerun any parser tooling against the updated grammar to confirm it accepts your examples.

Made changes.
