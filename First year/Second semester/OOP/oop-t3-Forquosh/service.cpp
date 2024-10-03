#include "service.h"

Service::Service(Repository &repository) : repository(repository) {}

std::vector<Equation> Service::getAllEquations() {
    return repository.getAllEquations();
}

void Service::addEquation(Equation eq) {
    repository.addEquation(eq);
}
