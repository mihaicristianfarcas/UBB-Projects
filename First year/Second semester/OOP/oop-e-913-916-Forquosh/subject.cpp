//
// Created by mihai on 28.06.2024.
//

#include "subject.h"

void Subject::registerObserver(Observer* observer) {
    observers.push_back(observer);
}

void Subject::notify() {
    for(auto o : observers)
        o->update();
}

