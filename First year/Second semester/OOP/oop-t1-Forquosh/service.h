//
// Created by mihai on 01.04.2024.
//
#pragma once
#include "repository.h"

class Services {

private:
    Repository& repository;

public:

    explicit Services(Repository& repository);

    ~Services();

    Player* getAllPlayers();

    int getSize();

    Player* getAllPlayersByNationality(const std::string& nationality);

    bool removePlayer(const std::string& name, const std::string& nationality, const std::string& team, int& goals);


};