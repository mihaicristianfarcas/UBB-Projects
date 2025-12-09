#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// IoTiny Runtime Support
void send_command(const char* device, const char* cmd) {
    printf("[COMMAND] %s: %s\n", device, cmd);
}

void trigger(const char* event, int value) {
    printf("[TRIGGER] %s -> %d\n", event, value);
}

int threshold = 25;

void check_temperature() {
}

// Event listener #1 for motion_sensor
void listener_1() {
    if (threshold > 0) {
        send_command("light", "on");
    }
}

int main() {
    printf("IoTiny Program Starting...\n");
    if (threshold > 20) {
    send_command("heater", "off");
    }
    if (threshold < 15) {
    send_command("heater", "on");
    }
    check_temperature();
    // for i in 3
    for (int i = 0; i < 3; i++) {
    send_command("led", "blink");
    }
    int temp = 30;
    int humidity = 65;
    if (temp > 28) {
    send_command("ac", "on");
    }
    return 0;
}
