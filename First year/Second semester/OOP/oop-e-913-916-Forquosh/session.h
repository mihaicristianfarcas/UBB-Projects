#pragma once
#include "subject.h"
#include "repository.h"

class Session : public Subject {
private:
    Repository& repository;

public:
    explicit Session(Repository& repo) : repository(repo) {}
    vector<Courier>& getCouriers();
    vector<Package>& getPackages();
    void addPackage(string recipient, string street, int number, int x, int y);


};