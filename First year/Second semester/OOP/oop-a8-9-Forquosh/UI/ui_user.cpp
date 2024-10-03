//#include "ui.h"
//
//UserUI::UserUI(User& user_service) : user_service(user_service) {
//        this->buildUIUser();
//        this->populateList();
//
//        QObject::connect(this->adoptButton, &QPushButton::clicked, this, &UserUI::adoptButtonHandler);
//        QObject::connect(this->nextButton, &QPushButton::clicked, this, &UserUI::nextButtonHandler);
//}
//
//void UserUI::populateList()
//{
//    this->dogListWidget->clear();
//
//    for (const auto& d : this->user_service.viewAllDogs())
//    {
////        std::string age = reinterpret_cast<const char *>(d.getAge());
//        auto* item = new QListWidgetItem{ QString::fromStdString(d.getBreed() + " - " + d.getName() + " - " + d.getPhotograph() )};
//        QFont font{ "Verdana", 10, 5 };
//        item->setFont(font);
//        this->dogListWidget->addItem(item);
//    }
//}
//
//void UserUI::adoptButtonHandler() {
//
//}
//
//void UserUI::nextButtonHandler() {
//
//}
//
//void UserUI::buildUIUser() {
//
//}
////
////void AdministratorUI::ui_user() {
////
////    int user_command;
////    std::string name;
////    std::string choice;
////
////    while (true) {
////        print_user_menu();
////        user_command = get_user_command();
////
////        if (user_command == EXIT_COMMAND) {
////            std::cout << "Thank you for visiting. Goodbye!\n";
////            break;
////        }
////
////        if (user_command == SEE_DOGS_COMMAND) {
////            auto dogs = this->user_service.viewAllDogs();
////            unsigned long long size = this->user_service.getRepoSize();
////            int i = 0;
////            while (i < size) {
////
////                std::cout << "Dog " << i + 1 << ":\n";
////                std::cout << "Breed: " << dogs[i].getBreed() << " ; Name: " << dogs[i].getName() << " ; Age: " \
////                << dogs[i].getAge() << " ;\nPhotograph URL: " << dogs[i].getPhotograph() << std::endl;
////
////                std::cout << "Do you want to adopt this dog? Yes/No\n";
////                std::cin >> choice;
////
////                if (choice == "Yes") {
////                    name = dogs[i].getName();
////                    if (this->user_service.adoptDog(name)) {
////                        std::cout << "Congratulations! You have successfully adopted " << name << ".\n";
////                    } else {
////                        std::cout << "Sorry, the dog with name " << name << " is not available for adoption.\n";
////                    }
////                }
////
////                i++;
////                if (i == size) {
////                    std::cout << "Do you want to go through the list again? Yes/No\n";
////                    std::cin >> choice;
////
////                    if (choice == "Yes")
////                        i = 0;
////                }
////            }
////        }
////
////        if (user_command == ADOPT_DOG_COMMAND) {
////            std::cout << "Enter the name of the dog you want to adopt: \n";
////            std::cin >> name;
////            if (this->user_service.adoptDog(name)) {
////                std::cout << "Congratulations! You have successfully adopted " << name << ".\n";
////            }
////            else {
////                std::cout << "Sorry, the dog with the name " << name << " is not available for adoption.\n";
////            }
////        }
////
////        if (user_command == SEE_ADOPTION_LIST_COMMAND) {
////            std::cout << "HTML or CSV?\n";
////            std::cin >> choice;
////            if (choice == "HTML") {
////                std::string command = "start " + htmlFile;
////                system(command.c_str());
////
////            } else if (choice == "CSV") {
////                std::string command = "start " + csvFile;
////                system(command.c_str());
////            }
////        }
////    }
////}