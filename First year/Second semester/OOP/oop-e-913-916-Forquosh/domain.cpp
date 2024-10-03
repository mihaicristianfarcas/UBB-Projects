//
// Created by mihai on 28.06.2024.
//

#include "domain.h"

string Courier::getName() {
    return name;
}

vector<string> &Courier::getStreets() {
    return assignedStreets;
}

Zone &Courier::getZone() {
    return zone;
}

string Package::getRecipient() {
    return recipient;
}

pair<string, int> Package::getAddress() {
    return address;
}

pair<int, int> Package::getLocation() {
    return location;
}

bool Package::getStatus() {
    return deliveryStatus;
}

string Package::toString() {
    string buffer;
    buffer = recipient + " - " + address.first + " ; " + to_string(address.second) + " - " + to_string(location.first) + ',' +
            to_string(location.second) + " - " + to_string(deliveryStatus);
    return buffer;
}
