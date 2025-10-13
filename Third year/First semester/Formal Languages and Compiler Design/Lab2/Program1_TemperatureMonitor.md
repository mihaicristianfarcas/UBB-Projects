# Program 1 — Temperature Monitor (IoTiny)

## Description

Simple IoTiny program that listens to a temperature sensor and turns a fan on when the temperature exceeds a threshold.

## IoTiny source

```
temp = 28

on "temp_sensor" when temp > 30:
    send_command "fan", "on"
end

# occasional check that logs or triggers (example expression statement)
if temp > 30:
    send_command "fan", "on"
end
```

## Which BNF constructs are used

- `listener_definition` (on ... when ... : ... end)
- `condition` (expression relop expression)
- `send_command` (device command invocation)
- `assignment` (identifier = expression)
- `if_statement` (if ... : ... end)

## Example runtime behaviours / expected outputs

- If the listener receives an event from `temp_sensor` where `temp` evaluates to 32, the runtime should execute the block and issue a command to the `fan` device: send_command("fan", "on").
- The assignment and `if` example shows a direct check: since `temp = 28` is not greater than 30, no command will be sent in that branch.
