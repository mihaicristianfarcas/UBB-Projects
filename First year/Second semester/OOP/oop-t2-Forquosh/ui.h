#pragma once
#include <iostream>
#include "service.h"
using namespace std;

class UI {

private:
    Service& service;

public:

    explicit UI(Service& service);

    void start();
};