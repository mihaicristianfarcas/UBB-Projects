# Program 2 — Light Schedule Controller (IoTiny)

## Description

IoTiny program that cycles lights on/off based on a schedule and allows manual override via a function call.

## IoTiny source

```
# function to set light to a given state
func set_light(device, state):
    send_command device, state
end

lights = ["light1", "light2", "light3"]

for d in lights:
    # example: schedule check — pretend `hour` is provided by environment
    if hour >= 18:
        set_light(d, "on")
    end
end

# manual override trigger
on "manual_override" when cmd == "lights_off":
    for d in lights:
        set_light(d, "off")
    end
end
```

## BNF constructs used

- `function_definition` / `function_call`
- `list_literal` and `for_loop`
- `assignment` and `expression_statement`
- `listener_definition`

## Example behaviours / expected outputs

- At runtime, when `hour` evaluates to 19, the for-loop will call `set_light` for each device, producing send_command("light1","on") etc.
- When an event named `manual_override` arrives with `cmd` equal to "lights_off", the listener will run and send off commands for each listed light.
