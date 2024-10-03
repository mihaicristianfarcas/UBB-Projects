//
// Created by mihai on 24.03.2024.
//
#pragma once

#include <vector>
#include "../repository/repository.h"

class Administrator {

private:
    Repository<Dog>& repository;

public:

    explicit Administrator(Repository<Dog>& repository);

    ~Administrator();

    bool addDog(std::string &breed, std::string &name, int age, std::string &photograph);

    bool deleteDog(std::string &name);

    bool updateDog(std::string &name, int newAge);

    int getRepoSize();

    Dog* viewAllDogs();
};

class User {

private:
    Repository<Dog>& repository;
    std::vector<Dog> adoptionList;

public:

    explicit User(Repository<Dog>& repository);

    ~User();

    int getRepoSize();

    Dog* viewAllDogs();

    bool adoptDog(const std::string& name);

    std::vector<Dog> seeAdoptionList();

};