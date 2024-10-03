//
// Created by mihai on 01.04.2024.
//
#include "domain.h"

Player::Player() {
    this->name = "";
    this->nationality = "";
    this->team = "";
    this->goals = 0;
}

Player::Player(std::string name, std::string nationality, std::string team, int goals) {
    this->name = name;
    this->nationality = nationality;
    this->team = team;
    this->goals = goals;
}

Player::~Player() = default;

std::string & Player::getName() { return this->name; }

std::string & Player::getNationality() { return this->nationality; }

std::string & Player::getTeam() { return this->team; }

int & Player::getGoals() { return this->goals; }