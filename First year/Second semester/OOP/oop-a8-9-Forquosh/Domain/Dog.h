//
// Created by mihai on 23.03.2024.
//
#pragma once
#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include <sstream>

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

    static std::vector<std::string> tokenize(const std::string& str, char delimiter) {
        std::vector<std::string> result;
        std::stringstream ss(str);
        std::string token;
        while(getline(ss, token, delimiter))
            result.push_back(token);

        return result;
    }

    friend std::ostream& operator<<(std::ostream& out, const Dog& dog) {
        out << dog.getBreed() << "," << dog.getName() << "," << dog.getAge() << "," << dog.getPhotograph();
        return out;
    }

    friend std::istream& operator>>(std::istream& in, Dog& dog) {
        std::string line;
        getline(in, line);

        std::vector<std::string> tokens = tokenize(line, ',');
        if (tokens.size() < 4)
            return in;

        dog = Dog(tokens[0], tokens[1], stoi(tokens[2]), tokens[3]);
        return in;
    }
};
