#include <QtWidgets/QApplication>
#include "gui.h"

int main(int argc, char** argv) {
    QApplication application(argc, argv);

    Repository repository;
    Service service{repository};
    GUI ui{service};

    ui.show();

    return QApplication::exec();
}