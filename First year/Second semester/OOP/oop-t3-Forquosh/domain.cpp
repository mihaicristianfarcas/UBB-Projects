#include "domain.h"

Equation::Equation(double a, double b, double c) : a(a), b(b), c(c) {}

Equation::Equation() : a(0), b(0), c(0) {}

double Equation::getA() const {
    return a;
}

double Equation::getB() const {
    return b;
}

double Equation::getC() const {
    return c;
}

std::pair<std::string,std::string> Equation::getSolution() const {
    std::pair<std::string ,std::string> solutions;

    double delta = b*b - (4*a*c);
    double solution1, solution2;
    if(delta > 0) {
        solution1 = (-b + std::sqrt(delta)) / 2*a;
        solutions.first = std::to_string(solution1);
        solution2 = (-b - std::sqrt(delta)) / 2*a;
        solutions.second = std::to_string(solution2);
    } else if (delta == 0) {
        solution1 = (-b + std::sqrt(delta)) / 2*a;
        solutions.first = std::to_string(solution1);
        solutions.second = std::to_string(solution1);
    } else if (delta < 0) {
        double negative_delta = -delta;
        solution1 = (-b + std::sqrt(negative_delta)) / 2*a;
        solutions.first = std::to_string(solution1) + "i";
        solution2 = (-b - std::sqrt(negative_delta)) / 2*a;
        solutions.second = std::to_string(solution2) + "i";
    }

    return solutions;
}

