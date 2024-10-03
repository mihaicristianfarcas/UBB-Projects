#include "service.h"

void Service::addBuilding(Building *b) {
    buildingRepository.push_back(b);
}

std::vector<Building *> Service::getAllBuildings() {
    return buildingRepository;
}

void Service::writeToFile(const std::string& filename, const std::vector<Building*>& buildings) {
    std::ofstream f(filename);
    for (auto building : buildings)
        f << building->toString() << '\n';
    f.close();
}

Service::~Service() {

    for(auto building : buildingRepository)
        delete building;
}

Service::Service() {
    auto a = new House{2002, false};
    auto b = new House{1909, true};
    auto c = new Block{2020, 40, 32};
    auto d = new Block{2004,30,29};

    addBuilding(a);
    addBuilding(b);
    addBuilding(c);
    addBuilding(d);
}

std::vector<Building*> Service::getAllToBeRestored() {
    std::vector<Building*> buildingsToBeRestored;
    for(auto building : buildingRepository) {
        if (building->mustBeRestored())
            buildingsToBeRestored.push_back(building);
    }
    return buildingsToBeRestored;
}

std::vector<Building*> Service::getAllToBeDemolished() {
    std::vector<Building*> buildingsToBeDemolished;
    for (auto building: buildingRepository) {
        if (building->canBeDemolished())
            buildingsToBeDemolished.push_back(building);
    }
    return buildingsToBeDemolished;
}
