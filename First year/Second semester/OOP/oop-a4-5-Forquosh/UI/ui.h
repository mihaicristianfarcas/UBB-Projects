//
// Created by mihai on 28.03.2024.
//
#pragma once
#include <iostream>
#include "../Services/services.h"
#include "../tests/tests.h"

class UI {

private:
    Administrator& administrator_service;
    User& user_service;

public:

    UI(Administrator& administrator_service, User& user_service) : administrator_service(administrator_service), user_service(user_service) {}

    ~UI() = default;

//    ADMINISTRATOR

    static void print_admin_menu();

    static int get_admin_command();

    static void get_breed(std::string &breed);

    static void get_name(std::string &name);

    static void get_age(int &age);

    static void get_photograph(std::string &photograph);

    static void get_dog_data(std::string &name, std::string &breed, int &age, std::string &photograph);

    void ui_admin();

//    USER

    static void print_user_menu();

    static int get_user_command();

    void ui_user();

};
