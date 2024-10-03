//
// Created by mihai on 03.06.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Chat.h" resolved

#include "chat_window.h"
#include "ui_Chat.h"


ChatWindow::ChatWindow(User& user, ChatSession& chat, QWidget *parent) : chatSession(chat), user(user), QWidget(parent), ui(new Ui::ChatWindow) {
    ui->setupUi(this);
    chatSession.registerObserver(this);
    setWindowTitle(QString::fromStdString(this->user.getUsername()));
    QPushButton::connect(this->ui->sendButton, &QPushButton::clicked, this, &ChatWindow::sendHandler);
}

void ChatWindow::update() {
    this->ui->messagesList->clear();
    for(auto msg: chatSession.getMessages()) {
        auto* item = new QListWidgetItem{QString::fromStdString(msg.getUser().getUsername() + ": " + msg.getMessage().getMessage() + '(' + std::to_string(msg.getMessage().getTimeStamp().tm_wday) + ')')};
        QFont font{"Verdana", 10, 5};
        item->setFont(font);
        if(msg.getUser().getUsername() == user.getUsername()) {
            item->setTextAlignment(Qt::AlignRight);
        }
        if(msg.getUser().getUsername() == "Diana") {
            item->setBackground(Qt::darkMagenta);
        }
        if(msg.getUser().getUsername() == "Mihai") {
            item->setBackground(Qt::darkCyan);
        }
        if(msg.getUser().getUsername() == "Alex") {
            item->setBackground(Qt::darkGreen);
        }

        this->ui->messagesList->addItem(item);
        ui->messageInput->clear();
    }
}

void ChatWindow::sendHandler(){
    QString text = this->ui->messageInput->text();
    tm now{};
    localtime_s(&now, nullptr);
    Message message{text.toStdString(), now};
    chatSession.addMessage(user, message);
}

ChatWindow::~ChatWindow() {
    delete ui;
}
