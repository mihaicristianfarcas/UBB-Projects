#pragma once

#include "polynomial.hpp"
#include <memory>
#include <string>

namespace metal_multiplier {

/// Initialize the Metal compute pipeline
/// Must be called before any other Metal operations
/// Returns true on success, false if Metal is not available
bool initialize();

/// Clean up Metal resources
void cleanup();

/// Check if Metal is available and initialized
bool is_available();

/// Get the name of the Metal device being used
std::string get_device_name();

/// Naive O(n²) polynomial multiplication on GPU
/// Uses parallel computation where each thread computes one result coefficient
Polynomial naive_gpu(const Polynomial& a, const Polynomial& b);

/// Karatsuba polynomial multiplication using GPU
/// The recursive structure runs on CPU, but the base-case multiplications
/// and element-wise operations (add, subtract, combine) run on GPU
Polynomial karatsuba_gpu(const Polynomial& a, const Polynomial& b, int threshold);

} // namespace metal_multiplier
