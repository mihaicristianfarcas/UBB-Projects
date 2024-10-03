#include "building.h"

Building::Building(int year) :  constructionYear(year) {}

std::string Building::toString() const {
    std::stringstream buffer;

    buffer << "Year of construction: " << constructionYear << '\n' << '\n';
    return buffer.str();
}

int Building::getYear() const {
    return constructionYear;
}

Building::~Building() = default;

