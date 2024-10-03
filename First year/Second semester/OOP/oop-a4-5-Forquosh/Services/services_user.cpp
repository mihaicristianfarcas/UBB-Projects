//
// Created by mihai on 23.03.2024.
//
#include "services.h"

User::User(Repository<Dog>& repository): repository(repository) {}

User::~User() = default;

/// Adds the dog to the adoption list
/// Returns True if it exists and is not already added
/// False otherwise
bool User::adoptDog(const std::string& name) {

    unsigned long long size = this->adoptionList.size();

    for (int i = 0; i < size; i++)
        if (this->adoptionList[i].getName() == name)
            return false;

    Dog* dogs = this->repository.getAllElements();
    size = this->repository.getLength();
    for (int i = 0; i < size; i++) {
        if (dogs[i].getName() == name) {
            adoptionList.push_back(dogs[i]);
            return true;
        }
    }

    return false;
}


std::vector<Dog> User::seeAdoptionList() {
    return this->adoptionList;
}

int User::getRepoSize() {
    return this->repository.getLength();
}

/// View all elements in the database
/// Returns the array of elements
Dog* User::viewAllDogs() {
    return this->repository.getAllElements();
}