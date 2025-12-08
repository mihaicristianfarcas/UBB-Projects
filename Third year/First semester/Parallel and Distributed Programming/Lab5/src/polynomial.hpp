#pragma once

#include <cstddef>
#include <random>
#include <string>
#include <utility>
#include <vector>

class Polynomial {
public:
    using coeff_t = long long;

    Polynomial() = default;
    explicit Polynomial(std::vector<coeff_t> coefficients);

    std::size_t size() const;
    int degree() const;
    const coeff_t& coefficient(std::size_t index) const;
    const std::vector<coeff_t>& coefficients() const;

    Polynomial add(const Polynomial& other) const;
    Polynomial subtract(const Polynomial& other) const;
    Polynomial slice(std::size_t start, std::size_t end) const;
    Polynomial shift(std::size_t k) const;
    bool is_zero() const;

    static Polynomial random(int degree, int bound, std::mt19937& rng);

private:
    std::vector<coeff_t> coeffs_{1, 0};
    void normalize();
};
