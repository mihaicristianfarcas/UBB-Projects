#pragma once
#include "string"
#include "sstream"

class Building {

protected:
    int constructionYear;

public:

    virtual ~Building();

    explicit Building(int year);

    int getYear() const;

    virtual bool mustBeRestored() = 0;

    virtual bool canBeDemolished() = 0;

    virtual  std::string toString() const;
};