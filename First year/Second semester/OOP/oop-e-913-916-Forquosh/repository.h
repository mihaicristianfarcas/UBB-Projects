#pragma once
#include <utility>

#include "domain.h"
class Repository {
private:
    vector<Courier> couriers;
    vector<Package> packages;
    string couriersFile, packagesFile;
    void readCouriers();
    void readPackages();
    void writeCouriers();
    void writePackages();

public:
    Repository(string couriers, string packages) : couriersFile(std::move(couriers)), packagesFile(std::move(packages)) { readCouriers(); readPackages(); }

    ~Repository() { writeCouriers(); writePackages(); }

    vector<Courier>& getCouriers();

    vector<Package>& getPackages();

    void addPackage(const Package& p);
};