#include "services.h"

User::User(Repository* repository): repository(repository) {}

User::~User() = default;

/// Adds the dog to the adoption list
/// Returns True if it exists and is not already added
/// False otherwise
bool User::adoptDog(const std::string& name) {

    for (const Dog& dog : this->adoptionList)
        if (dog.getName() == name)
            return false;

    Dog* doggo = this->repository->getDogByName(name);
    if (doggo) {
        adoptionList.push_back(*doggo);
        return true;
    }

    return false;
}


std::vector<Dog> User::seeAdoptionList() {
    return this->adoptionList;
}

unsigned long long User::getRepoSize() {
    return this->repository->getLength();
}

/// View all elements in the database
/// Returns the array of elements
std::vector<Dog> User::viewAllDogs() {
    return this->repository->getAllDogs();
}