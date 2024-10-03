//
// Created by mihai on 01.04.2024.
//
#include "ui.h"

int main() {
    Repository repository;
    Services service(repository);
    UI ui(service);

    ui.start();
}
