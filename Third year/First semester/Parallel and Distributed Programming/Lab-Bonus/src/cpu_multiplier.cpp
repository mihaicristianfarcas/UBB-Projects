#include "cpu_multiplier.hpp"

#include <algorithm>
#include <future>
#include <thread>
#include <utility>
#include <vector>

namespace {

std::vector<Polynomial::coeff_t> naive_core(const Polynomial& a, const Polynomial& b) {
    std::size_t result_size = a.size() + b.size() - 1;
    std::vector<Polynomial::coeff_t> result(result_size, 0);
    for (std::size_t i = 0; i < a.size(); ++i) {
        for (std::size_t j = 0; j < b.size(); ++j) {
            result[i + j] += a.coefficient(i) * b.coefficient(j);
        }
    }
    return result;
}

std::vector<Polynomial::coeff_t> compute_naive_chunk(const Polynomial& a, const Polynomial& b, 
                                                      std::size_t start, std::size_t end, 
                                                      std::size_t result_size) {
    std::vector<Polynomial::coeff_t> partial(result_size, 0);
    for (std::size_t i = start; i < end; ++i) {
        for (std::size_t j = 0; j < b.size(); ++j) {
            partial[i + j] += a.coefficient(i) * b.coefficient(j);
        }
    }
    return partial;
}

Polynomial karatsuba_seq_impl(const Polynomial& a, const Polynomial& b, int threshold) {
    std::size_t size = std::max(a.size(), b.size());
    if (size <= static_cast<std::size_t>(threshold)) {
        return Polynomial(naive_core(a, b));
    }

    std::size_t mid = size / 2;
    Polynomial a_low = a.slice(0, mid);
    Polynomial a_high = a.slice(mid, a.size());
    Polynomial b_low = b.slice(0, mid);
    Polynomial b_high = b.slice(mid, b.size());

    Polynomial z0 = karatsuba_seq_impl(a_low, b_low, threshold);
    Polynomial z2 = karatsuba_seq_impl(a_high, b_high, threshold);
    Polynomial z1 = karatsuba_seq_impl(a_low.add(a_high), b_low.add(b_high), threshold)
        .subtract(z0)
        .subtract(z2);

    return z0.add(z1.shift(mid)).add(z2.shift(2 * mid));
}

Polynomial karatsuba_par_impl(const Polynomial& a, const Polynomial& b, int threshold, int depth_remaining) {
    std::size_t size = std::max(a.size(), b.size());
    if (size <= static_cast<std::size_t>(threshold)) {
        return Polynomial(naive_core(a, b));
    }

    std::size_t mid = size / 2;
    Polynomial a_low = a.slice(0, mid);
    Polynomial a_high = a.slice(mid, a.size());
    Polynomial b_low = b.slice(0, mid);
    Polynomial b_high = b.slice(mid, b.size());

    Polynomial z1 = karatsuba_par_impl(a_low.add(a_high), b_low.add(b_high), threshold, depth_remaining);

    if (depth_remaining <= 0) {
        Polynomial z0 = karatsuba_seq_impl(a_low, b_low, threshold);
        Polynomial z2 = karatsuba_seq_impl(a_high, b_high, threshold);
        Polynomial middle = z1.subtract(z0).subtract(z2);
        return z0.add(middle.shift(mid)).add(z2.shift(2 * mid));
    }

    auto future_z0 = std::async(std::launch::async, karatsuba_par_impl, a_low, b_low, threshold, depth_remaining - 1);
    auto future_z2 = std::async(std::launch::async, karatsuba_par_impl, a_high, b_high, threshold, depth_remaining - 1);

    Polynomial z0 = future_z0.get();
    Polynomial z2 = future_z2.get();
    Polynomial middle = z1.subtract(z0).subtract(z2);
    return z0.add(middle.shift(mid)).add(z2.shift(2 * mid));
}

} // namespace

namespace cpu_multiplier {

Polynomial naive_sequential(const Polynomial& a, const Polynomial& b) {
    return Polynomial(naive_core(a, b));
}

Polynomial naive_parallel(const Polynomial& a, const Polynomial& b, int threads) {
    std::size_t result_size = a.size() + b.size() - 1;
    int safe_threads = std::max(1, threads);
    std::size_t chunk = std::max<std::size_t>(1, a.size() / static_cast<std::size_t>(safe_threads));

    std::vector<std::future<std::vector<Polynomial::coeff_t>>> futures;
    futures.reserve(static_cast<std::size_t>(safe_threads));
    for (std::size_t start = 0; start < a.size(); start += chunk) {
        std::size_t end = std::min(a.size(), start + chunk);
        futures.emplace_back(std::async(std::launch::async, compute_naive_chunk, 
                                        std::cref(a), std::cref(b), start, end, result_size));
    }

    std::vector<Polynomial::coeff_t> aggregated(result_size, 0);
    for (auto& fut : futures) {
        auto partial = fut.get();
        for (std::size_t i = 0; i < result_size; ++i) {
            aggregated[i] += partial[i];
        }
    }
    return Polynomial(std::move(aggregated));
}

Polynomial karatsuba_sequential(const Polynomial& a, const Polynomial& b, int threshold) {
    return karatsuba_seq_impl(a, b, threshold);
}

Polynomial karatsuba_parallel(const Polynomial& a, const Polynomial& b, int threshold, int max_depth) {
    return karatsuba_par_impl(a, b, threshold, max_depth);
}

} // namespace cpu_multiplier
