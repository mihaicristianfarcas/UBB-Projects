//
// Created by mihai on 28.06.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_CourierGUI.h" resolved

#include <iostream>
#include "couriergui.h"
#include "ui_CourierGUI.h"


CourierGUI::CourierGUI(Session& session, Courier& courier, QWidget *parent) : session(session), courier(courier), QWidget(parent), ui(new Ui::CourierGUI) {
    ui->setupUi(this);
    session.registerObserver(this);
    populateList();
    addStreetsToComboBox();

    setWindowTitle(QString::fromStdString(this->courier.getName() + " - Zone ->" + this->courier.getZone().toString()));
    QPushButton::connect(ui->deliverButton, &QPushButton::clicked, this, &CourierGUI::deliverButtonSlot);
    QComboBox::connect(ui->comboBox, &QComboBox::currentTextChanged, this, &CourierGUI::modifyList);

}

CourierGUI::~CourierGUI() {
    delete ui;
}

void CourierGUI::update() {
    populateList();
}

void CourierGUI::populateList() {
    ui->packagesList->clear();
    ui->packagesList->addItem(QString::fromStdString(to_string(courier.getStreets().size())));
    for(auto p : session.getPackages()) {
        if(p.getStatus() == 0) {
            // TODO get packages in the zone as well
//            for(const auto& s : courier.getStreets())
//                ui->packagesList->addItem(QString::fromStdString(p.getAddress().first + " " + s));
//                if(p.getAddress().first == s) {
//                    auto* item = new QListWidgetItem(QString::fromStdString(p.toString()));
//                    ui->packagesList->addItem(item);
//                    break;
//                }
        }
    }
}

void CourierGUI::deliverButtonSlot() {

}

void CourierGUI::addStreetsToComboBox() {
    ui->comboBox->addItem("All");
    for(auto p : session.getPackages()) {
        auto s = p.getAddress().first;
        ui->comboBox->addItem(QString::fromStdString(s));
    }
}

void CourierGUI::modifyList() {
    auto currentText = ui->comboBox->currentText().toStdString();
    if(currentText != "All") {
        ui->packagesList->clear();
        for (auto p: session.getPackages()) {
            if (p.getStatus() == 0 && p.getAddress().first == currentText) {
                // TODO get packages in the zone as well
                auto *item = new QListWidgetItem(QString::fromStdString(p.toString()));
                ui->packagesList->addItem(item);
            }
        }
    }
}