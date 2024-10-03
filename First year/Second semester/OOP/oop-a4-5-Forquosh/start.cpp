//
// Created by mihai on 23.03.2024.
//
#include <iostream>
#include "Repository/repository.h"
#include "Services/services.h"
#include "UI/ui.h"

using namespace std;


int main() {
    int choice;
    Repository<Dog> repository{10};
    Administrator administrator{repository};
    User user{repository};
    UI ui{administrator, user};

    testDomain();
    testRepository();
    testServicesAdmin();
    testServicesUser();

    cout << "Who is using the program? (0 - Admin ; 1 - User)\n";
    cin >> choice;
    if (choice)
        ui.ui_user();
    else
        ui.ui_admin();
}
