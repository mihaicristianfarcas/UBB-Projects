#include "repository.h"

void Repository::loadFile() {
    std::ifstream file(filename);
    if (!file.is_open())
        return;

    std::string line;
    while (getline(file, line)) {
        std::vector<std::string> tokens;
        std::stringstream ss(line);
        std::string token;

        while (getline(ss, token, ','))
            tokens.push_back(token);

        if (tokens.size() < 3)
            continue;

        Equation eq(std::stoi(tokens[0]), std::stoi(tokens[1]), std::stoi(tokens[2]));
        equations.push_back(eq);
    }
    file.close();
}

std::vector<Equation> Repository::getAllEquations() {
    return equations;
}

Repository::Repository() {
    loadFile();
}

void Repository::addEquation(Equation eq) {
    equations.push_back(eq);
}
