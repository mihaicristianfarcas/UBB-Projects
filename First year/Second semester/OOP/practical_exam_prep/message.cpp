#include "message.h"

Message::Message(std::string message, tm& time) : message(std::move(message)), timeStamp(time) {}

std::string Message::getMessage() {
    return message;
}

tm Message::getTimeStamp() {
    return timeStamp;
}
