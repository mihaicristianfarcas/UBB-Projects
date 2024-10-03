#include "ui.h"

int main() {

    Service service;
    UI ui{service};

    ui.start();

    return 0;
}