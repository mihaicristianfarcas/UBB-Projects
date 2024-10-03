//
// Created by mihai on 01.04.2024.
//
#include "repository.h"

Repository::Repository() {
    this->capacity = 10;
    this->size = 0;
    this->players = new Player[this->capacity];
    Repository::defaultPlayers();
}

Repository::~Repository() {
    delete[] this->players;
}

void Repository::defaultPlayers() {
    Player player = Player("Nora_Monk", "NOR", "Larvik", 83);
    this->players[0] = player;
    player = Player("Cristina_Neagu", "ROM", "Buducnost", 82);
    this->players[1]  = player;
    player = Player("Isabelle", "SWE", "CSM_Bucuresti", 42);
    this->players[2]  = player;
    player = Player("Nerea_Pena", "ESP", "FTC-Rail", 90);
    this->players[3]  = player;
    player = Player("Maria_Popescu", "ROU", "Rostov", 53);
    this->players[4]  = player;
    this->size = 5;
}

Player* Repository::getAllPlayers() { return this->players; }

int Repository::getSize() const {return this->size;}

/// Removes a player from the Repository
/// returns True if the player has been successfully removed, False otherwise
bool Repository::removePlayer(Player player) {
    for (int i = 0; i < this->size; i++) {
        if (this->players[i].getName() == player.getName()) {
            for (int j = i; j < this->size - 1; j++) {
                this->players[j] = this->players[j+1];
            }
            this->size--;
            return true;
        }
    }
    return false;
}