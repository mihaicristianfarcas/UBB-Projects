//
// Created by mihai on 24.03.2024.
//
#pragma once
#include <vector>
#include "../repository/repository.h"

class Administrator {

private:
    Repository* repository;

public:

    explicit Administrator(Repository* repository);

    ~Administrator();

    bool addDog(const std::string &breed, const std::string &name, int age, const std::string &photograph);

    bool deleteDog(const std::string &name);

    bool updateDog(const std::string &name, int newAge);

    unsigned long long getRepoSize();

    const std::vector<Dog>& viewAllDogs();
};

class User {

private:
    Repository* repository;
    std::vector<Dog> adoptionList;

public:

    explicit User(Repository* repository);

    ~User();

    unsigned long long getRepoSize();

    std::vector<Dog> viewAllDogs();

    bool adoptDog(const std::string& name);

    std::vector<Dog> seeAdoptionList();
};