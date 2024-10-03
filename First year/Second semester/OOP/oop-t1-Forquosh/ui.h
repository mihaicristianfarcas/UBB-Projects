//
// Created by mihai on 01.04.2024.
//
#pragma once
#include "service.h"

class UI {

private:
    Services& services;

public:

    explicit UI(Services& services);

    ~UI();

    void print_menu();

    void start();

};