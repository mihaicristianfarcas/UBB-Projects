#include "ui.h"

AdministratorUI::AdministratorUI(Administrator& administrator_service) : administrator_service(administrator_service) {
    this->buildUIAdmin();
    this->populateList();

    QObject::connect(this->addButton, &QPushButton::clicked, this, &AdministratorUI::addButtonHandler);
    QObject::connect(this->deleteButton, &QPushButton::clicked, this, &AdministratorUI::deleteButtonHandler);
    QObject::connect(this->editButton, &QPushButton::clicked, this, &AdministratorUI::editButtonHandler);
}

void AdministratorUI::buildUIAdmin()
{
    auto* mainLayout = new QVBoxLayout{ this };
    this->dogListWidget = new QListWidget{};
    mainLayout->addWidget(dogListWidget);

    auto* rightLayout = new QGridLayout{};

    auto* breedLabel = new QLabel{ "Breed" };
    this->breedEdit = new QLineEdit{};
    auto* nameLabel = new QLabel{ "Name" };
    this->nameEdit = new QLineEdit{};
    auto* ageLabel = new QLabel{ "Age" };
    this->ageEdit = new QLineEdit{};
    auto* photoLabel = new QLabel{ "Photo" };
    this->photoEdit = new QLineEdit{};

    rightLayout->addWidget(breedLabel, 0, 0);
    rightLayout->addWidget(this->breedEdit, 0, 1);
    rightLayout->addWidget(nameLabel, 1, 0);
    rightLayout->addWidget(this->nameEdit, 1, 1);
    rightLayout->addWidget(ageLabel, 2, 0);
    rightLayout->addWidget(this->ageEdit, 2, 1);
    rightLayout->addWidget(photoLabel, 3, 0);
    rightLayout->addWidget(this->photoEdit, 3, 1);

    this->addButton = new QPushButton{ "Add" };
    this->deleteButton = new QPushButton{ "Delete" };
    this->editButton = new QPushButton{ "Edit" };

    rightLayout->addWidget(this->addButton, 4, 0);
    rightLayout->addWidget(this->deleteButton, 4, 1);
    rightLayout->addWidget(this->editButton, 4, 2);

    mainLayout->addLayout(rightLayout);
}

void AdministratorUI::populateList() {
    this->dogListWidget->clear();

    for (const auto& d : this->administrator_service.viewAllDogs())
    {
        auto* item = new QListWidgetItem{ QString::fromStdString(d.getBreed() + " - " + d.getName() + " - " + std::to_string(d.getAge()) + " - " + d.getPhotograph() )};
        QFont font{ "Verdana", 10, 5 };
        item->setFont(font);
        this->dogListWidget->addItem(item);
    }
}


void AdministratorUI::addButtonHandler() {
    QString breed = this->breedEdit->text();
    QString name = this->nameEdit->text();
    QString age = this->ageEdit->text();
    QString photograph = this->photoEdit->text();

    this->administrator_service.addDog(breed.toStdString(), name.toStdString(), age.toInt(), photograph.toStdString());
    this->populateList();
}

void AdministratorUI::deleteButtonHandler() {
    QString name = this->nameEdit->text();

    this->administrator_service.deleteDog(name.toStdString());
    this->populateList();
}

void AdministratorUI::editButtonHandler() {
    QString name = this->nameEdit->text();
    QString age = this->ageEdit->text();

    this->administrator_service.updateDog(name.toStdString(), age.toInt());
    this->populateList();
}

