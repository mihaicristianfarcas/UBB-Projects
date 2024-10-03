#pragma once
#include "building.h"

class Block : public Building {

private:
    int totalApartments, occupiedApartments;

public:

    Block(int constructionYear, int total, int occupied);

    bool mustBeRestored() override;

    bool canBeDemolished() override;

    [[nodiscard]] std::string toString() const override;
};