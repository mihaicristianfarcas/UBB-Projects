//
// Created by mihai on 01.04.2024.
//
#pragma once
#include "domain.h"

class Repository {

private:
    int capacity;
    int size;
    Player* players;

    void defaultPlayers();

public:

    Repository();

    ~Repository();

    Player* getAllPlayers();

    int getSize() const;

    bool removePlayer(Player player);

};