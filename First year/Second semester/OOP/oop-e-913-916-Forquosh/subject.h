#pragma once
#include "observer.h"
#include "vector"
using namespace std;

class Subject {
private:
    vector<Observer*> observers;

public:
    void registerObserver(Observer* observer);
    void notify();
};