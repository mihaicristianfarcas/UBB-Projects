#include "user_message.h"

#include <utility>

UserMessage::UserMessage(User &user, Message message) : user(user), message(std::move(message)) {}

Message& UserMessage::getMessage() {
    return message;
}

User &UserMessage::getUser() {
    return user;
}
