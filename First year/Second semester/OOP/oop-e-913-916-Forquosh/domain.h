#pragma once

#include <utility>
#include <vector>
#include <tuple>
#include "string"
using namespace std;

class Zone {
private:
    int x, y, R;
public:
    Zone(int x, int y, int R) : x(x), y(y), R(R) {}

    [[nodiscard]] int getX() const { return x; }

    [[nodiscard]] int getY() const { return y; }

    [[nodiscard]] int getR() const { return R; }

    [[nodiscard]] string toString() const { string buffer; buffer = " X:" + to_string(x) + "   Y: " + to_string(y) + "   Radius: " + to_string(R); return buffer;}
};

class Courier {
private:
    string name;
    vector<string> assignedStreets;
    Zone zone; // x, y, R

public:
    Courier(string name, vector<string> assignedStreets, Zone zone) : name(std::move(name)), assignedStreets(std::move(assignedStreets)), zone(zone) {}
    string getName();
    vector<string>& getStreets();
    Zone& getZone();
};

class Package {
private:
    string recipient;
    pair<string,int> address;
    pair<int,int> location;
    bool deliveryStatus;

public:
    Package(string recipient, pair<string,int> address, pair<int,int> location, bool deliveryStatus) : recipient(std::move(recipient)), address(std::move(address)), location(std::move(location)), deliveryStatus(deliveryStatus) {}
    string getRecipient();
    pair<string,int> getAddress();
    pair<int,int> getLocation();
    bool getStatus();
    string toString();
};



