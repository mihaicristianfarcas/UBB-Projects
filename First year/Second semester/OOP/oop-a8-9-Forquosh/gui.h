#ifndef OOP_A8_9_FORQUOSH_GUI_H
#define OOP_A8_9_FORQUOSH_GUI_H

#include <QWidget>
#include "services.h"


QT_BEGIN_NAMESPACE
namespace Ui { class GUI; }
QT_END_NAMESPACE

class GUI : public QWidget {
Q_OBJECT

public:
    explicit GUI(User &user_services, QWidget *parent = nullptr);

    void populateListOneByOne();

    void showAdoptionListHandler();

    void adoptButtonHandler();

    void nextButtonHandler();

    ~GUI() override;

private:
    Ui::GUI *ui;
    User &user_service;
    std::vector<Dog> dogsList;
    unsigned long long current_dog = 0;
};

#endif //OOP_A8_9_FORQUOSH_GUI_H
