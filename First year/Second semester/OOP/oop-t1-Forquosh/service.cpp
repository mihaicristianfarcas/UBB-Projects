//
// Created by mihai on 01.04.2024.
//
#include "service.h"

Services::Services(Repository &repository) : repository(repository) {}

Services::~Services() = default;

Player* Services::getAllPlayers() { return this->repository.getAllPlayers(); }

int Services::getSize() {return this->repository.getSize();}

Player* Services::getAllPlayersByNationality(const std::string& nationality) {
    Player *players = this->repository.getAllPlayers();
    int size = this->repository.getSize();
    for (int i = 0; i < size - 1; i++)
        for (int j = i; j < size; j++) {
            if (players[i].getNationality() == nationality && players[j].getNationality() == nationality)
                if (players[i].getGoals() < players[j].getGoals()) {
                    Player aux = players[i];
                    players[i] = players[j];
                    players[j] = aux;
                }
        }
    return players;
}

/// Creates a Player instance and sends it to the Repository to be removed
/// returns True if the player has been successfully removed, False otherwise
bool Services::removePlayer(const std::string &name, const std::string &nationality, const std::string &team,
                            int &goals) {
    Player player = Player(name, nationality, team, goals);
    return repository.removePlayer(player);
}
