#pragma once

#include "polynomial.hpp"

#include <cstddef>
#include <future>
#include <vector>

namespace multiplier {

Polynomial naive_sequential(const Polynomial& a, const Polynomial& b);
Polynomial naive_parallel(const Polynomial& a, const Polynomial& b, int threads);

Polynomial karatsuba_sequential(const Polynomial& a, const Polynomial& b, int threshold);
Polynomial karatsuba_parallel(const Polynomial& a, const Polynomial& b, int threshold, int max_depth);

} // namespace multiplier
