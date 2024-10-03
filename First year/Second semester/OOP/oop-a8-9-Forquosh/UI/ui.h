#pragma once
#include <iostream>
#include <cstdlib>
#include "../Services/services.h"
#include "../tests/tests.h"
#include <qwidget.h>
#include <qlistwidget.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qlabel.h>

class AdministratorUI : public QWidget {

private:
    Administrator& administrator_service;

    QListWidget* dogListWidget{};
    QLineEdit* breedEdit{};
    QLineEdit* nameEdit{};
    QLineEdit* ageEdit{};
    QLineEdit* photoEdit{};

    QPushButton* addButton{};
    QPushButton* deleteButton{};
    QPushButton* editButton{};

    void buildUIAdmin();

    void populateList();

public:

    explicit AdministratorUI(Administrator& administrator_service);

    ~AdministratorUI() override = default;

    void addButtonHandler();

    void deleteButtonHandler();

    void editButtonHandler();
};


//class UserUI : public QWidget {
//
//private:
//    User& user_service;
//
//    std::string htmlFile = "../dogs.html";
//    std::string csvFile = "../dogs.csv";
//
//    QListWidget* dogListWidget;
//    QPushButton* adoptButton;
//    QPushButton* nextButton;
//
//    void buildUIUser();
//
//    void populateList();
//
//    void adoptButtonHandler();
//
//    void nextButtonHandler();
//
//public:
//
//    explicit UserUI(User& user_service);
//
//    ~UserUI() = default;
//};

