#include "polynomial.hpp"
#include <algorithm>
#include <stdexcept>

Polynomial::Polynomial(std::vector<coeff_t> coefficients) : coeffs_(std::move(coefficients)) {
    if (coeffs_.empty()) {
        coeffs_ = {0};
    }
    normalize();
}

void Polynomial::normalize() {
    while (coeffs_.size() > 1 && coeffs_.back() == 0) {
        coeffs_.pop_back();
    }
}

std::size_t Polynomial::size() const {
    return coeffs_.size();
}

int Polynomial::degree() const {
    return static_cast<int>(coeffs_.size()) - 1;
}

const Polynomial::coeff_t& Polynomial::coefficient(std::size_t index) const {
    static const coeff_t zero = 0;
    return index < coeffs_.size() ? coeffs_[index] : zero;
}

const std::vector<Polynomial::coeff_t>& Polynomial::coefficients() const {
    return coeffs_;
}

Polynomial Polynomial::add(const Polynomial& other) const {
    std::size_t result_size = std::max(size(), other.size());
    std::vector<coeff_t> result(result_size, 0);
    for (std::size_t i = 0; i < result_size; ++i) {
        result[i] = coefficient(i) + other.coefficient(i);
    }
    return Polynomial(std::move(result));
}

Polynomial Polynomial::subtract(const Polynomial& other) const {
    std::size_t result_size = std::max(size(), other.size());
    std::vector<coeff_t> result(result_size, 0);
    for (std::size_t i = 0; i < result_size; ++i) {
        result[i] = coefficient(i) - other.coefficient(i);
    }
    return Polynomial(std::move(result));
}

Polynomial Polynomial::slice(std::size_t start, std::size_t end) const {
    if (start >= coeffs_.size()) {
        return Polynomial({0});
    }
    end = std::min(end, coeffs_.size());
    std::vector<coeff_t> result(coeffs_.begin() + start, coeffs_.begin() + end);
    return Polynomial(std::move(result));
}

Polynomial Polynomial::shift(std::size_t k) const {
    if (is_zero() || k == 0) {
        return *this;
    }
    std::vector<coeff_t> result(k + coeffs_.size(), 0);
    std::copy(coeffs_.begin(), coeffs_.end(), result.begin() + k);
    return Polynomial(std::move(result));
}

bool Polynomial::is_zero() const {
    return coeffs_.size() == 1 && coeffs_[0] == 0;
}

Polynomial Polynomial::random(int degree, int bound, std::mt19937& rng) {
    std::uniform_int_distribution<coeff_t> dist(1, bound);
    std::uniform_int_distribution<int> sign_dist(0, 1);
    std::vector<coeff_t> coefficients(degree + 1);
    for (int i = 0; i <= degree; ++i) {
        coefficients[i] = dist(rng) * (sign_dist(rng) ? 1 : -1);
    }
    if (coefficients.back() == 0) {
        coefficients.back() = 1;
    }
    return Polynomial(std::move(coefficients));
}

void Polynomial::send(int dest, int tag, MPI_Comm comm) const {
    int size_int = static_cast<int>(coeffs_.size());
    MPI_Send(&size_int, 1, MPI_INT, dest, tag, comm);
    MPI_Send(coeffs_.data(), size_int, MPI_LONG_LONG, dest, tag + 1, comm);
}

Polynomial Polynomial::receive(int source, int tag, MPI_Comm comm) {
    int size_int;
    MPI_Recv(&size_int, 1, MPI_INT, source, tag, comm, MPI_STATUS_IGNORE);
    std::vector<coeff_t> coefficients(size_int);
    MPI_Recv(coefficients.data(), size_int, MPI_LONG_LONG, source, tag + 1, comm, MPI_STATUS_IGNORE);
    return Polynomial(std::move(coefficients));
}

void Polynomial::broadcast(int root, MPI_Comm comm) {
    int size_int = static_cast<int>(coeffs_.size());
    MPI_Bcast(&size_int, 1, MPI_INT, root, comm);
    coeffs_.resize(size_int);
    MPI_Bcast(coeffs_.data(), size_int, MPI_LONG_LONG, root, comm);
}
