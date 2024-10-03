//
// Created by mihai on 03.06.2024.
//

#include "subject.h"

void Subject::registerObserver(Observer *observer) {
    observers.push_back(observer);
}

void Subject::unregisterObserver(Observer *observer) {
    auto it = observers.begin();
    for(auto o: observers) {
        if(o == observer) {
            observers.erase(it);
            break;
        }
        it++;
    }
    Subject::notify();
}

void Subject::notify() {
    for(auto o: observers) {
        o->update();
    }
}
