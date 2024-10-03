//
// Created by mihai on 27.05.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GUI.h" resolved

#include "gui.h"
#include "ui_GUI.h"


GUI::GUI(Service& service, QWidget *parent) : service(service), QWidget(parent), ui(new Ui::GUI) {
    ui->setupUi(this);
    populateList();

    QObject::connect(this->ui->solutionButton, &QPushButton::clicked, this, &GUI::solutionButtonHandler);
    QObject::connect(this->ui->addEquationButton, &QPushButton::clicked, this, &GUI::addEquationButtonHandler);

}

void GUI::populateList() {

    this->ui->equationList->clear();

    for(const auto& eq : service.getAllEquations()) {
        int a = eq.getA();
        int b = eq.getB();
        int c = eq.getA();

        std::string A, B, C;

        A = std::to_string(a);

        if(b >= 0) {
            B = "+ " + std::to_string(b);
        } else {
            B = std::to_string(b);
        }

        if(c >= 0) {
            C = "+ " + std::to_string(c);
        } else {
            C = std::to_string(c);
        }

        std::pair<std::string,std::string> solution = eq.getSolution();

        auto* item = new QListWidgetItem{QString::fromStdString(A + "*x^2 " + B + "*x " + C)};
        QFont font{ "Verdana", 10, 5 };
        item->setFont(font);
        this->ui->equationList->addItem(item);
    }
}


GUI::~GUI() {
    delete ui;
}

void GUI::solutionButtonHandler() {
    QString a = this->ui->lineA->text();
    QString b = this->ui->lineB->text();
    QString c = this->ui->lineC->text();

    Equation eq{a.toDouble(), b.toDouble(), c.toDouble()};

    std::pair<std::string,std::string> solution = eq.getSolution();

    this->ui->solutionLabel->setText(QString::fromStdString("Solutions are: " + solution.first + " ; " + solution.second));
}

void GUI::addEquationButtonHandler() {
    QString a = this->ui->lineA->text();
    QString b = this->ui->lineB->text();
    QString c = this->ui->lineC->text();

    Equation eq{a.toDouble(), b.toDouble(), c.toDouble()};
    service.addEquation(eq);
    populateList();
}
