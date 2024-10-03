//
// Created by mihai on 27.05.2024.
//

#ifndef OOP_T3_FORQUOSH_GUI_H
#define OOP_T3_FORQUOSH_GUI_H

#include <QWidget>
#include "service.h"


QT_BEGIN_NAMESPACE
namespace Ui { class GUI; }
QT_END_NAMESPACE

class GUI : public QWidget {
Q_OBJECT

public:
    GUI(Service& service, QWidget *parent = nullptr);

    ~GUI() override;

private:
    Ui::GUI *ui;
    Service& service;

    void populateList();

    void solutionButtonHandler();

    void addEquationButtonHandler();
};


#endif //OOP_T3_FORQUOSH_GUI_H
