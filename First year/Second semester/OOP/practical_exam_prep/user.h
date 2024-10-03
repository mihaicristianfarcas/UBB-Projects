#pragma once

#include <string>
#include <utility>

class User {

private:
    std::string uniqueUserName;
    std::string name;

public:

    User(std::string username, std::string realname);

    std::string getUsername();
};