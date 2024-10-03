#pragma once
#include "building.h"

class House : public Building {

private:
    std::string type;
    bool isHistorical;

public:
    House(int constructionYear, bool historical);

    bool mustBeRestored() override;

    bool canBeDemolished() override;

    [[nodiscard]] std::string toString() const override;
};