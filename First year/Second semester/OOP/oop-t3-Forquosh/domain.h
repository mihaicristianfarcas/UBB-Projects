#pragma once

#include <utility>
#include <string>
#include "cmath"

class Equation {

private:
    double a, b ,c;

public:

    Equation(double a, double b, double c);

    Equation();

    [[nodiscard]] double getA() const;

    [[nodiscard]] double getB() const;

    [[nodiscard]] double getC() const;

    [[nodiscard]] std::pair<std::string,std::string> getSolution() const;
};