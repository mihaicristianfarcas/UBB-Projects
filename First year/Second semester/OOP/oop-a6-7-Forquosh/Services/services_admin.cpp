//
// Created by mihai on 15.04.2024.
//
#include "services.h"

// Constructor
Administrator::Administrator(Repository* repository) : repository{repository} {}

Administrator::~Administrator() = default;

// Adds a new dog to the database
// Returns true if the dog has been successfully added, false otherwise
bool Administrator::addDog(const std::string& breed, const std::string& name, int age, const std::string& photograph) {
    for (const Dog& dog : this->repository->getAllDogs()) {
        if (dog.getName() == name) {
            return false;
        }
    }

    Dog newDog(breed, name, age, photograph);
    this->repository->addDog(newDog);
    return true;
}

// Deletes a dog from the database
// Returns true if the dog has been successfully deleted, false otherwise
bool Administrator::deleteDog(const std::string& name) {
    auto& dogs = this->repository->getAllDogs();
    auto it = std::find_if(dogs.begin(), dogs.end(), [&name](const Dog& dog) { return dog.getName() == name; });
    if (it != dogs.end()) {
        this->repository->removeDog(name);
        return true;
    }
    return false;
}

// Updates information of a dog in the database
// Returns true if the information has been successfully updated, false otherwise
bool Administrator::updateDog(const std::string& name, int newAge) {
    Dog* dog = this->repository->getDogByName(name);
    if (dog) {
        dog->setAge(newAge);
        this->repository->saveToFile("dogs.txt");
        return true;
    }
    return false;
}


// Returns the repository size
unsigned long long Administrator::getRepoSize() {
    return this->repository->getLength();
}

// View all elements in the database
// Returns the vector of elements
const std::vector<Dog>& Administrator::viewAllDogs() {
    return this->repository->getAllDogs();
}
