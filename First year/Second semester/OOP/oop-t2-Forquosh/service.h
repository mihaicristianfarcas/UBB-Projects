#pragma once
#include "vector"
#include "fstream"
#include "building.h"
#include "house.h"
#include "block.h"

class Service {

private:
    std::vector<Building*> buildingRepository;

public:
    Service();

    ~Service();

    void addBuilding(Building* b);

    std::vector<Building*> getAllBuildings();

    std::vector<Building*> getAllToBeRestored();

    std::vector<Building*> getAllToBeDemolished();

    static void writeToFile(const std::string& filename, const std::vector<Building*>& buildings);
};