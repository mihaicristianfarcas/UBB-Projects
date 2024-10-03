#include <iostream>
#include <QApplication>
#include "session.h"
#include "couriergui.h"
#include "companygui.h"
using namespace std;

int main(int argc, char** argv) {
    QApplication application(argc, argv);

    Repository repository{"../couriers.txt", "../packages.txt"};
    Session session{repository};
    for(auto c : session.getCouriers()) {
        auto courierWindow = new CourierGUI{session, c};
        courierWindow->show();
    }

    auto companyWindow = new companyGUI{session};
    companyWindow->show();

    return QApplication::exec();
}