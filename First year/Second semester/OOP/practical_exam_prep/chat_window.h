//
// Created by mihai on 03.06.2024.
//

#ifndef SEMINAR7_CHAT_WINDOW_H
#define SEMINAR7_CHAT_WINDOW_H

#include <QWidget>
#include "chat_session.h"


QT_BEGIN_NAMESPACE
namespace Ui { class ChatWindow; }
QT_END_NAMESPACE

class ChatWindow : public QWidget, public Observer {
Q_OBJECT

public:
    ChatWindow(User& user, ChatSession& chat, QWidget *parent = nullptr);

    void update() override;

    void sendHandler();

    ~ChatWindow() override;

private:
    Ui::ChatWindow *ui;
    User& user;
    ChatSession& chatSession;
};


#endif //SEMINAR7_CHAT_WINDOW_H
