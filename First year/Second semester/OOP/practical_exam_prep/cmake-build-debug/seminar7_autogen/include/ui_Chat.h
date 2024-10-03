/********************************************************************************
** Form generated from reading UI file 'chat.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHAT_H
#define UI_CHAT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChatWindow
{
public:
    QVBoxLayout *verticalLayout;
    QListWidget *messagesList;
    QLineEdit *messageInput;
    QPushButton *sendButton;

    void setupUi(QWidget *ChatWindow)
    {
        if (ChatWindow->objectName().isEmpty())
            ChatWindow->setObjectName("ChatWindow");
        ChatWindow->resize(400, 295);
        verticalLayout = new QVBoxLayout(ChatWindow);
        verticalLayout->setObjectName("verticalLayout");
        messagesList = new QListWidget(ChatWindow);
        messagesList->setObjectName("messagesList");

        verticalLayout->addWidget(messagesList);

        messageInput = new QLineEdit(ChatWindow);
        messageInput->setObjectName("messageInput");

        verticalLayout->addWidget(messageInput);

        sendButton = new QPushButton(ChatWindow);
        sendButton->setObjectName("sendButton");

        verticalLayout->addWidget(sendButton);


        retranslateUi(ChatWindow);

        QMetaObject::connectSlotsByName(ChatWindow);
    } // setupUi

    void retranslateUi(QWidget *ChatWindow)
    {
        ChatWindow->setWindowTitle(QCoreApplication::translate("ChatWindow", "ChatWindow", nullptr));
        sendButton->setText(QCoreApplication::translate("ChatWindow", "Send", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChatWindow: public Ui_ChatWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHAT_H
