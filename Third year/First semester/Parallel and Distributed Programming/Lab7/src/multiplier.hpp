#pragma once

#include "polynomial.hpp"
#include <mpi.h>

namespace multiplier {

// Sequential implementations (for comparison and fallback)
Polynomial naive_sequential(const Polynomial& a, const Polynomial& b);
Polynomial karatsuba_sequential(const Polynomial& a, const Polynomial& b, int threshold);

// MPI-distributed implementations
Polynomial naive_mpi(const Polynomial& a, const Polynomial& b, int rank, int size);
Polynomial karatsuba_mpi(const Polynomial& a, const Polynomial& b, int threshold, int rank, int size);

} // namespace multiplier
