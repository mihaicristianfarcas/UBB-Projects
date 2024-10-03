#pragma once

#include <ctime>
#include <utility>
#include <string>

class Message {

private:
    tm timeStamp;
    std::string message;

public:
    explicit Message(std::string message, tm& time);

    std::string getMessage();

    tm getTimeStamp();
};