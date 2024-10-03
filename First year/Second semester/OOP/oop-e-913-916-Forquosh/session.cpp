//
// Created by mihai on 28.06.2024.
//

#include "session.h"

#include <utility>

vector<Courier> &Session::getCouriers() {
    return repository.getCouriers();
}

vector<Package> &Session::getPackages() {
    return repository.getPackages();
}

void Session::addPackage(string recipient, string street, int number, int x, int y) {
    pair<string,int> streetAndNumber{street, number};
    pair<int,int> location{x,y};
    Package p{std::move(recipient), streetAndNumber, location, 0};
    repository.addPackage(p);
    notify();
}
