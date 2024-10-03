//
// Created by mihai on 01.04.2024.
//
#pragma once
#include "string"

class Player {

private:
    std::string name;
    std::string nationality;
    std::string team;
    int goals;

public:

    Player();

    Player(std::string name, std::string nationality, std::string team, int goals);

    ~Player();

    std::string & getName();

    std::string & getNationality();

    std::string & getTeam();

    int& getGoals();

};