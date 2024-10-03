//
// Created by mihai on 23.03.2024.
//
#include "services.h"

/// Constructor
Administrator::Administrator(Repository<Dog>& repository) : repository{repository} {}

Administrator::~Administrator() = default;

/// Adds a new dog to the database
/// Returns true if the dog has been successfully added, false otherwise
bool Administrator::addDog(std::string& breed, std::string& name, int age, std::string& photograph) {

    for (int i = 0; i < this->repository.getLength(); i++) {
        Dog& dog = this->repository.getElementAt(i);
        if (dog.getName() == name) {
            return false;
        }
    }

    Dog newDog(breed, name, age, photograph);
    this->repository.addElement(newDog);
    return true;
}

/// Deletes a dog from the database
/// Returns true if the dog has been successfully deleted, false otherwise
bool Administrator::deleteDog(std::string& name) {

    bool found = false;
    for (int i = 0; i < this->repository.getLength(); i++) {
        Dog& dog = this->repository.getElementAt(i);
        if (dog.getName() == name) {
            this->repository.removeElement(name);
            found = true;
            break;
        }
    }

    return found;
}

/// Updates information of a dog in the database
/// Returns true if the information has been successfully updated, false otherwise
bool Administrator::updateDog(std::string& name, int newAge) {

    bool found = false;
    for (int i = 0; i < this->repository.getLength(); i++) {
        Dog& dog = this->repository.getElementAt(i);
        if (dog.getName() == name) {
            dog.setAge(newAge);
            found = true;
            break;
        }
    }

    return found;
}

int Administrator::getRepoSize() {
    return this->repository.getLength();
}
/// View all elements in the database
/// Returns the array of elements
Dog* Administrator::viewAllDogs() {
    return this->repository.getAllElements();
}
