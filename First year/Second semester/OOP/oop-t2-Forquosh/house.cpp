#include "house.h"

House::House(int constructionYear, bool historical) : Building{constructionYear}, isHistorical(historical) {}

bool House::canBeDemolished() {
    return !isHistorical;
}

bool House::mustBeRestored() {
    return (2024 - constructionYear) > 100;
}

std::string House::toString() const {
    std::string baseStr = Building::toString();

    std::stringstream buffer;
    buffer << "HOUSE!\n" << "Historical: " << isHistorical << '\n';

    return buffer.str() + baseStr;
}