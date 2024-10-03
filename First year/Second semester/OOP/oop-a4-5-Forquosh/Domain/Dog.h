//
// Created by mihai on 23.03.2024.
//
#pragma once
#include <string>

class Dog {

private:
    std::string breed;
    std::string name;
    int age;
    std::string photograph;

public:

    Dog();

    Dog(const std::string &breed, const std::string &name, const int &age, const std::string &photograph);

    std::string getBreed() const;

    std::string getName() const;

    int getAge() const;

    void setAge(int newAge);

    std::string getPhotograph() const;

};

