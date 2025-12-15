#pragma once

#include <cstddef>
#include <random>
#include <vector>
#include <mpi.h>

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

    // MPI serialization helpers
    void send(int dest, int tag, MPI_Comm comm) const;
    static Polynomial receive(int source, int tag, MPI_Comm comm);
    void broadcast(int root, MPI_Comm comm);

private:
    std::vector<coeff_t> coeffs_{1, 0};
    void normalize();
};
