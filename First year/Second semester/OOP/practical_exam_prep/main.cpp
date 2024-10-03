#include <QtWidgets/QApplication>
#include "chat_window.h"

int main(int argc, char* argv[]) {
    QApplication application(argc, argv);

    ChatSession session;
    User user1("Mihai", "Mihai");
    User user2("Alex", "Alex");
    User user3("Diana", "Diana");

    ChatWindow chatWindow1(user1, session);
    ChatWindow chatWindow2(user2, session);
    ChatWindow chatWindow3(user3, session);

    chatWindow1.show();
    chatWindow2.show();
    chatWindow3.show();

    return QApplication::exec();
}
