# IoTiny — A Minimal Domain-Specific Language for IoT Automation

## 1. Overview

**IoTiny** is a small, readable, and intuitive **domain-specific language (DSL)** designed to express **IoT automation behaviors**.
It allows developers and engineers to **define reactions to sensor events**, **send commands to devices**, and **create simple logic flows** using an event-driven model.

Its syntax emphasizes **clarity**, **minimalism**, and **human readability**, making it suitable for prototyping IoT workflows or expressing device logic in constrained environments.

IoTiny scripts use **explicit `end` delimiters** to close control blocks, ensuring structure and simplicity.

## 2. Core Language Concepts

| Concept                | Description                                                | Example                                        |
| ---------------------- | ---------------------------------------------------------- | ---------------------------------------------- |
| **Event-driven logic** | Define reactions to named events (like temperature, light) | `on "temperature" when temp > 30: ... end`     |
| **Device commands**    | Send commands to devices (e.g., turn on/off, set state)    | `send_command "fan", "on"`                     |
| **Conditions**         | Express if-statements to make decisions                    | `if temp < 5: send_command "heater", "on" end` |
| **Assignments**        | Store or update simple variables                           | `threshold = 25`                               |
| **Triggers**           | Manually trigger events for testing                        | `"temperature" -> 35`                          |

## 3. Example IoTiny Script

```plaintext
# IoTiny Example: Smart Home Automation

threshold = 25

on "temperature" when temp > threshold:
    send_command "fan", "on"
end

on "light" when level < 10:
    send_command "lamp", "on"
end

# Simulated event triggers
"temperature" -> 32
"light" -> 5
```

**Explanation:**

- The script defines a **temperature listener** that turns on the fan when the temperature exceeds the threshold.
- A second listener monitors light levels and activates the lamp when brightness drops below 10.
- `"event" -> value` simulates an event trigger, such as sensor input.

## 4. Language Statements and Examples

## 4.1. Event Listener

**Syntax:**

```plaintext
on "event_name" when <condition>:
    <block>
end
```

**Purpose:**
Defines an action block that executes when a specific event occurs **and** the given condition evaluates to true.

**Example:**

```plaintext
on "humidity" when level > 60:
    send_command "dehumidifier", "on"
end
```

**Why it’s needed:**
This construct allows reactive programming — devices respond automatically to sensor conditions, a core pattern in IoT systems.

## 4.2. Send Command

**Syntax:**

```plaintext
send_command "device", "command"
```

**Purpose:**
Sends an action command to a device — for example, turning on/off actuators.

**Example:**

```plaintext
send_command "fan", "off"
```

**Why it’s needed:**
Represents communication from controller logic to IoT devices — the essence of automation.

## 4.3. Assignment

**Syntax:**

```plaintext
<identifier> = <expression>
```

**Purpose:**
Stores values that can be used in expressions or conditions.

**Example:**

```plaintext
threshold = 28
```

**Why it’s needed:**
Allows scripts to remain flexible and configurable without hardcoding constants.

## 4.4. Conditional Statement

**Syntax:**

```plaintext
if <condition>:
    <block>
end
```

**Example:**

```plaintext
if temp > 30:
    send_command "fan", "on"
end
```

**Why it’s needed:**
Allows decision-making logic independent of events — useful for direct evaluation of sensor values.

## 4.5. Trigger Call

**Syntax:**

```plaintext
"event_name" -> <value>
```

**Purpose:**
Simulates the generation of an event. Useful for testing or simulating sensor behavior.

**Example:**

```plaintext
"temperature" -> 35
```

**Why it’s needed:**
Enables developers to test their automation scripts even without physical sensors.

## 5. Example Use Case: Smart Environment Controller

**Scenario:**
A smart home system that automatically controls devices based on temperature and light sensors.

**IoTiny Script:**

```plaintext
threshold = 22

on "temperature" when temp > threshold:
    send_command "fan", "on"
end

on "temperature" when temp < threshold:
    send_command "heater", "on"
end

on "light" when level < 15:
    send_command "lamp", "on"
end

# Test run
"temperature" -> 18
"light" -> 8
```

**Explanation:**

- The script defines **three event listeners**:

  - Turns on the **fan** when it’s too hot
  - Turns on the **heater** when it’s too cold
  - Turns on the **lamp** when light levels are low

- The **triggers** simulate these environmental changes.

## 6. Design Rationale

- **Minimal Syntax:** Focused on core IoT concepts — events, conditions, and actions.
- **Readable:** Plain-English-like syntax for non-programmer domain experts.
- **Event-Driven:** Built around `on ... when ... end` blocks to mirror real IoT event flows.
- **Portable:** Not bound to specific devices or frameworks — conceptual and educational.
