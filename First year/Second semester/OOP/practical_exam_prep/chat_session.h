#pragma once

#include <vector>
#include "user_message.h"
#include "subject.h"

class ChatSession : public Subject {

private:
    std::vector<UserMessage> messages;

public:

    void addMessage(User& user, Message message);

    std::vector<UserMessage> getMessages();
};