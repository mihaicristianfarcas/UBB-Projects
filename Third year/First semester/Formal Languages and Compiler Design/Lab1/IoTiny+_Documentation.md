# IoTiny+ Documentation

## 1. Overview

**IoTiny+** is a **domain-specific language (DSL)** designed for **Internet of Things (IoT) automation and control**.
It provides a clean, minimal, and readable syntax for expressing event-driven behaviors, automation rules, and device control logic.

## 2. Language Philosophy

1. **Clarity** – Code should read like natural instructions.
   Example:

   ```plaintext
   on "temperature" when temp > 30:
       send_command "fan", "on"
   end
   ```

2. **Modularity** – Functions and loops promote reuse and structure.

3. **Reactivity** – Built around real-world IoT patterns of sensors and events.

## 3. Core Concepts

| Concept        | Description                                    | Example                              |
| -------------- | ---------------------------------------------- | ------------------------------------ |
| **Events**     | Define reactive behaviors when an event occurs | `on "motion" when detected == True:` |
| **Conditions** | Specify thresholds or rules                    | `if temp > 30:`                      |
| **Actions**    | Commands sent to devices                       | `send_command "fan", "on"`           |
| **Variables**  | Store configuration or sensor values           | `threshold = 25`                     |
| **Functions**  | Group reusable logic                           | `func cool_down(device): ... end`    |
| **Loops**      | Iterate over devices or collections            | `for d in devices: ... end`          |

## 4. Example IoTiny+ Script

```plaintext
# IoTiny+ Example: Smart Room Controller

threshold = 25
devices = ["lamp", "fan"]

func cool_down(device):
    send_command device, "on"
end

func report_status():
    for d in devices:
        send_command d, "status"
    end
end

on "temperature" when temp > threshold:
    cool_down "fan"
    report_status()
end

on "light" when level < 10:
    send_command "lamp", "on"
end

# Simulated events
"temperature" -> 30
"light" -> 5
```

### Explanation

- The program defines **two event listeners** reacting to temperature and light levels.
- **Functions** are used to modularize logic (`cool_down` and `report_status`).
- A **for loop** iterates over device names to send commands.
- **Simulated events** (`"temperature" -> 30`) test the behavior.

## 5. Statement Descriptions and Use-Case Examples

Each of the following statements has been designed to express a common IoT behavior concisely and logically.

### 5.1. Event Listener

**Syntax:**

```plaintext
on "event_name" when <condition>:
    <block>
end
```

**Purpose:**
Defines an action block that executes when an event occurs and the condition is true.

**Example:**

```plaintext
on "temperature" when temp > 30:
    send_command "fan", "on"
end
```

**Why it’s needed:**
IoT systems are inherently event-driven. This construct lets scripts react automatically to sensor inputs, mimicking real IoT event handling patterns.

### 5.2. Send Command

**Syntax:**

```plaintext
send_command <expression>, <expression>
```

**Purpose:**
Sends a command to a device — such as turning a fan on or a lamp off.

**Example:**

```plaintext
send_command "fan", "off"
```

**Why it’s needed:**
Represents the actuation step in IoT — where the controller instructs devices to perform actions.

### 5.3. Variable Assignment

**Syntax:**

```plaintext
<identifier> = <expression>
```

**Purpose:**
Stores a configurable or dynamic value (like a threshold or sensor reading).

**Example:**

```plaintext
threshold = 25
```

**Why it’s needed:**
Enables flexibility — allows IoTiny+ scripts to adapt to different conditions without rewriting logic.

### 5.4. Conditional Statement

**Syntax:**

```plaintext
if <condition>:
    <block>
end
```

**Example:**

```plaintext
if temp < 15:
    send_command "heater", "on"
end
```

**Why it’s needed:**
Adds decision-making to IoTiny+, letting scripts choose actions dynamically depending on sensor data.

### 5.5. For Loop

**Syntax:**

```plaintext
for <identifier> in <expression>:
    <block>
end
```

**Example:**

```plaintext
for device in ["lamp", "fan"]:
    send_command device, "status"
end
```

**Why it’s needed:**
Allows iteration over device lists, a common requirement for managing multiple IoT nodes at once.

### 5.6. Function Definition

**Syntax:**

```plaintext
func <name>(<parameters>):
    <block>
end
```

**Example:**

```plaintext
func cool_down(device):
    send_command device, "on"
end
```

**Why it’s needed:**
Promotes code reuse and modularization. Common IoT tasks (like device startup, alerting, or cleanup) can be abstracted into reusable functions.

### 5.7. Function Call

**Syntax:**

```plaintext
<identifier> (<arguments>)
```

**Example:**

```plaintext
cool_down "fan"
```

**Why it’s needed:**
Lets developers execute reusable blocks of logic, increasing clarity and maintainability.

### 5.8. Event Trigger

**Syntax:**

```plaintext
"event_name" -> <value>
```

**Purpose:**
Simulates or initiates an event, providing testability or direct triggering behavior.

**Example:**

```plaintext
"temperature" -> 30
```

**Why it’s needed:**
IoTiny+ scripts can be tested without real sensors, making simulation and debugging easier.

## 6. Example Use Case: Smart Greenhouse Automation

### Scenario:

A greenhouse monitors temperature and humidity to maintain optimal conditions.
If temperature exceeds 28°C, the cooling system turns on.
If humidity falls below 40%, the misting system activates.
Every few cycles, it reports the status of all devices.

**IoTiny+ Script:**

```plaintext
threshold_temp = 28
threshold_humidity = 40
devices = ["cooler", "mister", "lamp"]

func report_status():
    for d in devices:
        send_command d, "status"
    end
end

on "temperature" when temp > threshold_temp:
    send_command "cooler", "on"
    report_status()
end

on "humidity" when level < threshold_humidity:
    send_command "mister", "on"
end

"temperature" -> 30
"humidity" -> 35
```

**Explanation:**

- Uses **functions** for reporting.
- Uses **loops** to manage multiple devices.
- Uses **conditional events** to react to environment data.

## 7. Design Rationale

| Goal                   | How IoTiny+ Achieves It                                                  |
| ---------------------- | ------------------------------------------------------------------------ |
| **Domain specificity** | Built explicitly for IoT workflows — events, conditions, device commands |
| **Simplicity**         | Minimal keywords, readable syntax, explicit `end` delimiters             |
| **Reactivity**         | Event-driven model (`on ... when ... end`)                               |
| **Modularity**         | Functions and loops for reusable, structured code                        |
| **Testability**        | Manual event triggers simulate real-world sensors                        |
| **Extensibility**      | New device actions or network primitives can be easily added             |

## 8. Conclusion

**IoTiny+** represents a small yet expressive **IoT scripting DSL** that combines the readability of Python with the structured clarity of Lua or Ruby.

It:

- Defines both **lexical** and **syntactic** rules (via BNF)
- Includes **statements** relevant to IoT logic
- Provides clear **examples** and **justifications**
- Supports **functions**, **loops**, **conditionals**, and **events**

This makes IoTiny+ a practical foundation for educational compiler projects, IoT simulators, or embedded automation frameworks.
