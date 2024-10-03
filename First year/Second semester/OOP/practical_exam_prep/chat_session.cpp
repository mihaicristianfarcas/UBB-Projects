#include "chat_session.h"

void ChatSession::addMessage(User &user, Message message) {
    UserMessage new_message(user, message);
    messages.push_back(new_message);
    notify();
}

std::vector<UserMessage> ChatSession::getMessages() {
    return messages;
}
