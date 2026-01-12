#include "polynomial.hpp"

#include <algorithm>
#include <random>
#include <vector>

Polynomial::Polynomial(std::vector<coeff_t> coefficients) : coeffs_(std::move(coefficients)) {
    normalize();
}

std::size_t Polynomial::size() const {
    return coeffs_.size();
}

int Polynomial::degree() const {
    return static_cast<int>(coeffs_.size()) - 1;
}

const Polynomial::coeff_t& Polynomial::coefficient(std::size_t index) const {
    static const coeff_t zero = 0;
    if (index >= coeffs_.size()) {
        return zero;
    }
    return coeffs_[index];
}

const std::vector<Polynomial::coeff_t>& Polynomial::coefficients() const {
    return coeffs_;
}

std::vector<Polynomial::coeff_t>& Polynomial::coefficients_mut() {
    return coeffs_;
}

Polynomial Polynomial::add(const Polynomial& other) const {
    std::size_t max_size = std::max(size(), other.size());
    std::vector<coeff_t> result(max_size, 0);
    for (std::size_t i = 0; i < max_size; ++i) {
        result[i] = coefficient(i) + other.coefficient(i);
    }
    return Polynomial(std::move(result));
}

Polynomial Polynomial::subtract(const Polynomial& other) const {
    std::size_t max_size = std::max(size(), other.size());
    std::vector<coeff_t> result(max_size, 0);
    for (std::size_t i = 0; i < max_size; ++i) {
        result[i] = coefficient(i) - other.coefficient(i);
    }
    return Polynomial(std::move(result));
}

Polynomial Polynomial::slice(std::size_t start, std::size_t end) const {
    if (start >= coeffs_.size() || start >= end) {
        return Polynomial({0});
    }
    end = std::min(end, coeffs_.size());
    std::vector<coeff_t> part(coeffs_.begin() + static_cast<long>(start), coeffs_.begin() + static_cast<long>(end));
    return Polynomial(std::move(part));
}

Polynomial Polynomial::shift(std::size_t k) const {
    if (is_zero()) {
        return *this;
    }
    std::vector<coeff_t> shifted(k, 0);
    shifted.insert(shifted.end(), coeffs_.begin(), coeffs_.end());
    return Polynomial(std::move(shifted));
}

bool Polynomial::is_zero() const {
    return coeffs_.size() == 1 && coeffs_[0] == 0;
}

Polynomial Polynomial::random(int degree, int bound, std::mt19937& rng) {
    std::uniform_int_distribution<int> dist(1, bound);
    std::bernoulli_distribution sign_dist(0.5);
    std::vector<coeff_t> result(static_cast<std::size_t>(degree) + 1);
    for (int i = 0; i <= degree; ++i) {
        int magnitude = dist(rng);
        int sign = sign_dist(rng) ? 1 : -1;
        result[static_cast<std::size_t>(i)] = sign * magnitude;
    }
    return Polynomial(std::move(result));
}

void Polynomial::normalize() {
    while (coeffs_.size() > 1 && coeffs_.back() == 0) {
        coeffs_.pop_back();
    }
}
