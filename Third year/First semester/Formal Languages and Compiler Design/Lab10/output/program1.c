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

int temp = 28;

// Event listener #1 for temp_sensor
void listener_1() {
    if (temp > 30) {
        send_command("fan", "on");
    }
}

int main() {
    printf("IoTiny Program Starting...\n");
    if (temp > 30) {
    send_command("fan", "on");
    }
    return 0;
}
