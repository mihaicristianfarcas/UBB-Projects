#pragma once

#include "polynomial.hpp"

#include <cstddef>
#include <future>
#include <vector>

namespace cpu_multiplier {

// Sequential implementations
Polynomial naive_sequential(const Polynomial& a, const Polynomial& b);
Polynomial karatsuba_sequential(const Polynomial& a, const Polynomial& b, int threshold);

// Parallel implementations (CPU threads)
Polynomial naive_parallel(const Polynomial& a, const Polynomial& b, int threads);
Polynomial karatsuba_parallel(const Polynomial& a, const Polynomial& b, int threshold, int max_depth);

} // namespace cpu_multiplier
