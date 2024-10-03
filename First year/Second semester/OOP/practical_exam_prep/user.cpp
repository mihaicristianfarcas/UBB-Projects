#include "user.h"

User::User(std::string username, std::string realname) : uniqueUserName(std::move(username)), name(std::move(realname)) {}

std::string User::getUsername() {
    return uniqueUserName;
}
