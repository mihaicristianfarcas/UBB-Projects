//
// Created by mihai on 23.03.2024.
//
#include "ui.h"

const int ADD_COMMAND = 1, DELETE_COMMAND = 2, UPDATE_COMMAND = 3, DISPLAY_COMMAND = 4, EXIT_COMMAND = 0;


void UI::print_admin_menu() {
    std::cout << "\nHello Administrator! Please choose an option:\n";
    std::cout << "1. Add a dog!\n";
    std::cout << "2. Remove a dog!\n";
    std::cout << "3. Update a dog's data!\n";
    std::cout << "4. Show all elements!\n";
    std::cout << "0. Stop program.\n";
}

int UI::get_admin_command() {
    int command;
    std::cin >> command;
    if (command < 0 || command > 4) {
        std::cout << "Invalid command! Please try again...\n";
        return -1;
    }

    return command;
}

void UI::get_breed(std::string &breed) {
    std::cout << "\nBreed: ";
    std::cin >> breed;
}

void UI::get_name(std::string &name) {
    std::cout << "\nName: ";
    std::cin >> name;
}

void UI::get_age(int &age) {
    std::cout << "\nAge: ";
    std::cin >> age;
}

void UI::get_photograph(std::string &photograph) {
    std::cout << "\nPhotograph URL: ";
    std::cin >> photograph;
}

void UI::get_dog_data(std::string &name, std::string &breed, int &age, std::string &photograph) {
    std::cout << "Please enter the dog's data:";
    get_breed(breed);
    get_name(name);
    get_age(age);
    get_photograph(photograph);
}

void UI::ui_admin() {

    int user_command;

    std::string breed, name, photograph;
    int age;

    while(true) {
        print_admin_menu();
        user_command = get_admin_command();

        if (user_command == EXIT_COMMAND) {
            std::cout << "Bye!";
            break;
        }

        if (user_command == ADD_COMMAND) {
            get_dog_data(name, breed, age, photograph);
            this->administrator_service.addDog(breed, name, age, photograph);
        }

        if (user_command == DELETE_COMMAND) {
            get_name(name);
            this->administrator_service.deleteDog(name);
        }

        if (user_command == UPDATE_COMMAND) {
            get_name(name);
            get_age(age);
            this->administrator_service.updateDog(name, age);
        }

        if (user_command == DISPLAY_COMMAND) {
            int i = 0;
            for (const Dog& dog : this->administrator_service.viewAllDogs()) {
                std::cout << "Dog " << i + 1 << ":\n";
                std::cout << "Breed: " << dog.getBreed() << " ; Name: " << dog.getName() << " ; Age: " \
                << dog.getAge() << " ;\nPhotograph URL: " << dog.getPhotograph() << std::endl;
                i++;
            }
        }

    }
}

