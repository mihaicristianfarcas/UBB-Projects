#include "ui.h"

UI::UI(Service& service) : service(service) {}

void UI::start() {

    while (true) {
        cout << "Please choose an option:\n";
        cout << "1. Add a building!\n";
        cout << "2. Show all buildings!\n";
        cout << "3. Show all buildings that must be restored, built in a given year!\n";
        cout << "4. Save the buildings to a file!\n";

        int choice;
        cin >> choice;

        if (choice == 1) {
            int type, year;
            cout << "What kind? 1 - House ; 2 - Block\n";
            cin >> type;

            cout << "Please give the construction year:\n";
            cin >> year;

            if (type == 1) {
                bool historical;
                cout << "Historical? 1 - Yes ; 0 - No\n";
                cin >> historical;

                auto house = new House{year, historical};
                service.addBuilding(house);
            } else {
                int total, occupied;
                cout << "How many apartments?\n";
                cin >> total;
                cout << "Of which occupied?\n";
                cin >>occupied;

                auto block = new Block{year, total, occupied};
                service.addBuilding(block);
            }

        }

        if (choice == 2) {
            for(auto b : service.getAllBuildings())
                cout << b->toString();
        }

        if (choice == 3) {
            int year;
            cout << "Give the year:\n";
            cin >> year;
            for(auto b : service.getAllToBeRestored()){
                if(b->getYear() == year)
                    cout << b->toString();
            }
        }

        if (choice == 4) {
            Service::writeToFile("toDemolish.txt", service.getAllToBeDemolished());
            Service::writeToFile("toRestore.txt", service.getAllToBeRestored());
        }

        if (choice == 0) {
            cout << "Bye!";
            break;
        }

    }
}