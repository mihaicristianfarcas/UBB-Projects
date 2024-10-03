//
// Created by mihai on 28.06.2024.
//

#include <fstream>
#include <sstream>
#include "repository.h"

vector<Courier> &Repository::getCouriers() {
    return couriers;
}

vector<Package> &Repository::getPackages() {
    return packages;
}

void Repository::readCouriers() {
    ifstream file(couriersFile);
    string line;
    while(getline(file,line)) {

        // adding whole line delim. by '/' in result
        stringstream ss(line);
        string token;
        vector<string> result;
        while(getline(ss, token, '/')) {
            result.push_back(token);
        }

        // getting name
        string name = result[0];

        // getting streets
        stringstream sstreets(result[1]);
        vector<string> streets;
        while(getline(sstreets, token, ',')) {
            streets.push_back(token);
        }

        // getting zone
        string x, y, R;
        stringstream szone(result[2]);
        getline(szone, x, ',');
        getline(szone, y, ',');
        getline(szone, R, ',');
        Zone z{stoi(x), stoi(y), stoi(R)};

        // constructing Courier
        Courier c{name, streets, z};
        couriers.push_back(c);
    }
    file.close();
}

void Repository::readPackages() {
    ifstream file(packagesFile);
    string line;
    while(getline(file,line)) {

        // adding whole line delim. by '/' in result
        stringstream ss(line);
        string token;
        vector<string> result;
        while (getline(ss, token, '/')) {
            result.push_back(token);
        }

        // getting recipient
        string recipient = result[0];

        // getting street and number
        string street, number;
        stringstream sstreetNumber(result[1]);
        getline(sstreetNumber, street, ',');
        getline(sstreetNumber, number, ',');

        pair<string, int> address;
        address.first = street;
        address.second = stoi(number);

        // getting location
        string x, y;
        stringstream sLocation(result[2]);
        getline(sLocation, x, ',');
        getline(sLocation, y, ',');

        pair<int, int> location;
        location.first = stoi(x);
        location.second = stoi(y);

        // getting status
        bool status = stoi(result[3]);

        // constructing Package
        Package p{recipient, address, location, status};
        packages.push_back(p);
    }
    file.close();
}

void Repository::writeCouriers() {
    ofstream file(couriersFile);
    for(auto c : couriers) {
        file << c.getName() << '/';
        for(auto it = c.getStreets().begin(); it < c.getStreets().end(); it++) {
            if(!(it == c.getStreets().end() - 1))
                file << *it << ',';
            else
                file << *it;
        }
        file << '/' << c.getZone().getX() << ',' << c.getZone().getY() << ',' << c.getZone().getR() << '\n';
    }
    file.close();
}

void Repository::writePackages() {
    ofstream file(packagesFile);
    for(auto p : packages) {
        file << p.getRecipient() << '/';
        file << p.getAddress().first << ',' << p.getAddress().second << '/';
        file << p.getLocation().first << ',' << p.getLocation().second << '/';
        file << p.getStatus() << '\n';
    }
    file.close();
}

void Repository::addPackage(const Package& p) {
    packages.push_back(p);
}
