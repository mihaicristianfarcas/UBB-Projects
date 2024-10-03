#include "block.h"

Block::Block(int constructionYear, int total, int occupied) : Building(constructionYear), totalApartments(total), occupiedApartments(occupied) {}

bool Block::mustBeRestored() {
    return (2024 - constructionYear > 40) && (occupiedApartments > ((totalApartments * 4) / 5));
}

bool Block::canBeDemolished() {
    return (occupiedApartments * 20 < totalApartments);
}

std::string Block::toString() const {
    std::string baseStr = Building::toString();

    std::stringstream buffer;
    buffer << "BLOCK!\n" << "Total nr. of apartments: " << totalApartments << '\n' << "Of which occupied: " << occupiedApartments << '\n';

    return buffer.str() + baseStr;
}
