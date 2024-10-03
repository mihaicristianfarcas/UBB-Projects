#pragma once

#include "user.h"
#include "message.h"

class UserMessage {

private:
    User& user;
    Message message;

public:
    UserMessage(User& user, Message message);

    Message& getMessage();

    User& getUser();
};