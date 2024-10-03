#include <iostream>
#include <QtWidgets/QApplication>
#include "Repository/repository.h"
#include "Services/services.h"
#include "UI/ui.h"
#include "gui.h"

using namespace std;

int main(int argc, char** argv) {

    QApplication application(argc, argv);
    int choice;

    HTMLFileRepository repositoryHTML{"../dogs.html"};
    CSVFileRepository repositoryCSV{"../dogs.csv"};

    // Repository of choice
    Repository* repo = &repositoryHTML;

    // Services
    Administrator administrator{repo};
    User user{repo};

    // GUI
    AdministratorUI AdminGUI{administrator};

    // GUI w/ QT Designer
    GUI UserGUI{user};

    cout << "Who is using the program? (0 - Admin ; 1 - User)\n";
    cin >> choice;

    if (choice)
        UserGUI.show();
    else
        AdminGUI.show();

    return QApplication::exec();
}
