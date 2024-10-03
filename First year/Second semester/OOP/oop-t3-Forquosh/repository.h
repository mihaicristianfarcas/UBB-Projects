#pragma once
#include "fstream"
#include "sstream"
#include "vector"
#include "domain.h"

class Repository {

private:
    std::vector<Equation> equations;
    std::string filename = "../equations.txt";

    void loadFile();

public:

    Repository();

    std::vector<Equation> getAllEquations();

    void addEquation(Equation eq);
};