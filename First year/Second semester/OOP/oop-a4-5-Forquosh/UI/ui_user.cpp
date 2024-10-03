//
// Created by mihai on 23.03.2024.
//
#include "ui.h"

const int SEE_DOGS_COMMAND = 1, ADOPT_DOG_COMMAND = 2, SEE_ADOPTION_LIST_COMMAND = 3, EXIT_COMMAND = 0;


void UI::print_user_menu() {
    std::cout << "\nWelcome to the adoption center! Please choose an option:\n";
    std::cout << "1. See dogs available for adoption.\n";
    std::cout << "2. Adopt a dog.\n";
    std::cout << "3. See adoption list.\n";
    std::cout << "0. Exit.\n";
}

int UI::get_user_command() {
    int command;
    std::cin >> command;
    if (command < 0 || command > 3) {
        std::cout << "Invalid command! Please try again...\n";
        return -1;
    }

    return command;
}

void UI::ui_user() {

    int user_command;
    std::string name;
    std::string choice;

    while (true) {
        print_user_menu();
        user_command = get_user_command();

        if (user_command == EXIT_COMMAND) {
            std::cout << "Thank you for visiting. Goodbye!\n";
            break;
        }

        if (user_command == SEE_DOGS_COMMAND) {
            Dog* dogs = this->user_service.viewAllDogs();
            int size = this->user_service.getRepoSize();
            int i = 0;
            while (i < size) {

                std::cout << "Dog " << i + 1 << ":\n";
                std::cout << "Breed: " << dogs[i].getBreed() << " ; Name: " << dogs[i].getName() << " ; Age: " \
                << dogs[i].getAge() << " ;\nPhotograph URL: " << dogs[i].getPhotograph() << std::endl;

                std::cout << "Do you want to adopt this dog? Yes/No\n";
                std::cin >> choice;

                if (choice == "Yes") {
                    name = dogs[i].getName();
                    if (this->user_service.adoptDog(name)) {
                        std::cout << "Congratulations! You have successfully adopted " << name << ".\n";
                    } else {
                        std::cout << "Sorry, the dog with name " << name << " is not available for adoption.\n";
                    }
                }

                i++;
                if (i == size) {
                    std::cout << "Do you want to go through the list again? Yes/No\n";
                    std::cin >> choice;

                    if (choice == "Yes")
                        i = 0;
                }
            }
        }

        if (user_command == ADOPT_DOG_COMMAND) {
            std::cout << "Enter the name of the dog you want to adopt: \n";
            std::cin >> name;
            if (this->user_service.adoptDog(name)) {
                std::cout << "Congratulations! You have successfully adopted " << name << ".\n";
            }
            else {
                std::cout << "Sorry, the dog with the name " << name << " is not available for adoption.\n";
            }
        }

        if (user_command == SEE_ADOPTION_LIST_COMMAND) {
            std::cout << "Adoption List:\n";
            std::vector<Dog> adoption_list = this->user_service.seeAdoptionList();
            for (const Dog& dog : adoption_list ) {
                std::cout << "Breed: " << dog.getBreed() << " ; Name: " << dog.getName() << " ; Age: " \
                << dog.getAge() << " ;\nPhotograph URL: " << dog.getPhotograph() << std::endl;
            }
        }
    }
}