#pragma once

#include "observer.h"
#include "vector"

class Subject {

private:
    std::vector<Observer*> observers;

public:

    void registerObserver(Observer* observer);

    void unregisterObserver(Observer* observer);

    void notify();
};