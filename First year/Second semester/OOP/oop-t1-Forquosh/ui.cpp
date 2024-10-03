//
// Created by mihai on 01.04.2024.
//
#include "ui.h"
#include <iostream>
using namespace std;

UI::UI(Services &services) : services(services){}

UI::~UI() = default;

void UI::print_menu() {
    cout << "Hello! Please choose an option:\n";
    cout << "1. Remove a player!\n";
    cout << "2. Show all players!\n";
    cout << "3. Show all players of a given nationality!\n";
}

void UI::start() {
    int user_choice;
    string name;
    string nationality;
    string team;
    int goals;

    while(true) {
        print_menu();
        cin >> user_choice;

        if(user_choice == 1) {
            cout << "Please give the player's data (name, nationality, team , goals):\n";
            cin >> name;
            cin >> nationality;
            cin >> team;
            cin >> goals;
            if (services.removePlayer(name, nationality, team, goals)) {
                cout << "Player removed!\n";
            }
            else {
                cout << "Player could not be removed. Please try again...";
            }
        }

        if(user_choice == 2) {
            Player* players = this->services.getAllPlayers();
            int size = this->services.getSize();
            for (int i = 0; i < size; i++)
                cout << players[i].getName() << " | " << players[i].getNationality() << " | " << players[i].getTeam() << " | " << players[i].getGoals() << endl;
        }

        if(user_choice == 3) {
            cout << "Please give the nationality:\n";
            cin >> nationality;
            Player *players = this->services.getAllPlayersByNationality(nationality);
            int i = 0;
            while (&players[i]) {
                cout << players[i].getName() << " | " << players[i].getNationality() << " | " << players[i].getTeam()
                     << " | " << players[i].getGoals() << endl;
                i++;
            }
        }
        }


}