#pragma once
#include "repository.h"

class Service {

private:
    Repository& repository;

public:

    explicit Service(Repository& repository);

    std::vector<Equation> getAllEquations();

    void addEquation(Equation eq);
};