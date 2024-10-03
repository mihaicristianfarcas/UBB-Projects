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

    HTMLFileRepository repositoryHTML{"../dogs.html"};
    CSVFileRepository repositoryCSV{"../dogs.csv"};

    Repository* repo = &repositoryHTML;

    Administrator administrator{repo};
    User user{repo};
    UI ui{administrator, user};

    cout << "Who is using the program? (0 - Admin ; 1 - User)\n";
    cin >> choice;
    if (choice)
        ui.ui_user();
    else
        ui.ui_admin();
}
