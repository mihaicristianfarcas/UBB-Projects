//
// Created by mihai on 23.03.2024.
//
#include "Dog.h"

Dog::Dog(const std::string &breed, const std::string &name, const int &age, const std::string &photograph) {
    this->breed = breed;
    this->name = name;
    this->age = age;
    this->photograph = photograph;
}

Dog::Dog() {
    this->breed = "-";
    this->name = "-";
    this->age = 0;
    this->photograph = "-";
}

std::string Dog::getBreed() const { return this->breed; }

std::string Dog::getName() const { return this->name; }

int Dog::getAge() const { return this->age; }

void Dog::setAge(int newAge) { this->age = newAge; }

std::string Dog::getPhotograph() const { return this->photograph; }
