//
// Created by mihai on 28.06.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_companyGUI.h" resolved

#include "companygui.h"
#include "ui_companyGUI.h"


companyGUI::companyGUI(Session& session, QWidget *parent) : session(session), QWidget(parent), ui(new Ui::companyGUI) {
    ui->setupUi(this);
    setWindowTitle(QString::fromStdString("Company"));
    populateList();

    QPushButton::connect(ui->addButton, &QPushButton::clicked, this, &companyGUI::addPackageSlot);
}

companyGUI::~companyGUI() {
    delete ui;
}

void companyGUI::update() {
    populateList();
}

void companyGUI::populateList() {
    ui->allPackagesList->clear();
    for(auto p : session.getPackages()) {
        auto* item = new QListWidgetItem(QString::fromStdString(p.toString()));
        if(p.getStatus() == 1) {
            item->setBackground(Qt::darkGreen);
        }
        ui->allPackagesList->addItem(item);
    }
}

void companyGUI::addPackageSlot() {
    auto recipient = ui->recipientLine->text().toStdString();
    auto street = ui->streetLine->text().toStdString();
    auto number = ui->numberLine->text().toStdString();
    auto x = ui->XLine->text().toStdString();
    auto y = ui->YLine->text().toStdString();

    ui->recipientLine->clear();
    ui->streetLine->clear();
    ui->numberLine->clear();
    ui->XLine->clear();
    ui->YLine->clear();

    session.addPackage(recipient, street, stoi(number), stoi(x), stoi(y));
    populateList();
}
