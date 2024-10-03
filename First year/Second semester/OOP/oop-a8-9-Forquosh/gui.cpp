#include "gui.h"
#include "ui_GUI.h"


GUI::GUI(User& user_services, QWidget *parent) : user_service(user_services), QWidget(parent), ui(new Ui::GUI) {
    ui->setupUi(this);
    dogsList = this->user_service.viewAllDogs();
    populateListOneByOne();

    QObject::connect(this->ui->showAdoptionListButton, &QPushButton::clicked, this, &GUI::showAdoptionListHandler);
    QObject::connect(this->ui->adoptButton, &QPushButton::clicked, this, &GUI::adoptButtonHandler);
    QObject::connect(this->ui->nextDogButton, &QPushButton::clicked, this, &GUI::nextButtonHandler);
}

GUI::~GUI() {
    delete ui;
}

void GUI::populateListOneByOne() {
    this->ui->dogsList->clear();

    auto d = dogsList[current_dog];
    auto* item = new QListWidgetItem{ QString::fromStdString(d.getBreed() + " - " + d.getName() + " - " + std::to_string(d.getAge()) + " - " + d.getPhotograph())};
    QFont font{ "Verdana", 10, 5 };
    item->setFont(font);

    this->ui->dogsList->addItem(item);

}

void GUI::showAdoptionListHandler() {
    this->ui->dogsList->clear();

    for (const auto& d : this->user_service.seeAdoptionList())
    {
        auto* item = new QListWidgetItem{ QString::fromStdString(d.getBreed() + " - " + d.getName() + " - " + std::to_string(d.getAge()) + " - " + d.getPhotograph() )};
        QFont font{ "Verdana", 10, 5 };
        item->setFont(font);
        this->ui->dogsList->addItem(item);
    }
}

void GUI::adoptButtonHandler() {
    this->user_service.adoptDog(dogsList[current_dog].getName());
    nextButtonHandler();
}

void GUI::nextButtonHandler() {
    current_dog++;

    if (current_dog == dogsList.size())
        current_dog = 0;

    populateListOneByOne();
}
