#include "multiplier.hpp"
#include <algorithm>
#include <vector>
#include <cmath>

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

} // namespace

namespace multiplier {

Polynomial naive_sequential(const Polynomial& a, const Polynomial& b) {
    return Polynomial(naive_core(a, b));
}

Polynomial karatsuba_sequential(const Polynomial& a, const Polynomial& b, int threshold) {
    return karatsuba_seq_impl(a, b, threshold);
}

Polynomial naive_mpi(const Polynomial& a, const Polynomial& b, int rank, int world_size) {
    std::size_t result_size = a.size() + b.size() - 1;
    
    // Divide work: each process handles a chunk of rows from polynomial a
    std::size_t chunk_size = (a.size() + world_size - 1) / world_size;
    std::size_t start = rank * chunk_size;
    std::size_t end = std::min(start + chunk_size, a.size());
    
    // Compute partial result for this process's chunk
    std::vector<Polynomial::coeff_t> partial(result_size, 0);
    for (std::size_t i = start; i < end; ++i) {
        for (std::size_t j = 0; j < b.size(); ++j) {
            partial[i + j] += a.coefficient(i) * b.coefficient(j);
        }
    }
    
    // Reduce all partial results to rank 0
    std::vector<Polynomial::coeff_t> result(result_size, 0);
    MPI_Reduce(partial.data(), result.data(), static_cast<int>(result_size), 
               MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    
    // Broadcast result from rank 0 to all processes
    MPI_Bcast(result.data(), static_cast<int>(result_size), 
              MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    
    return Polynomial(std::move(result));
}

Polynomial karatsuba_mpi(const Polynomial& a, const Polynomial& b, int threshold, int rank, int world_size) {
    std::size_t size = std::max(a.size(), b.size());
    
    // Base case: use naive algorithm
    if (size <= static_cast<std::size_t>(threshold)) {
        return naive_mpi(a, b, rank, world_size);
    }
    
    std::size_t mid = size / 2;
    
    // Split polynomials
    Polynomial a_low = a.slice(0, mid);
    Polynomial a_high = a.slice(mid, a.size());
    Polynomial b_low = b.slice(0, mid);
    Polynomial b_high = b.slice(mid, b.size());
    
    // Prepare cross-term polynomials
    Polynomial a_sum = a_low.add(a_high);
    Polynomial b_sum = b_low.add(b_high);
    
    // Distribute work among processes for the three multiplications
    // We'll use a simple strategy: assign different subproblems to different process groups
    
    if (world_size >= 3) {
        // Divide processes into 3 groups for z0, z1, z2
        int group_size = world_size / 3;
        int remainder = world_size % 3;
        
        int group_id;
        int group_rank;
        int group_world_size;
        
        if (rank < group_size + (remainder > 0 ? 1 : 0)) {
            // Group 0: compute z0
            group_id = 0;
            group_rank = rank;
            group_world_size = group_size + (remainder > 0 ? 1 : 0);
        } else if (rank < 2 * group_size + (remainder > 1 ? 2 : remainder > 0 ? 1 : 0)) {
            // Group 1: compute z1
            group_id = 1;
            group_rank = rank - (group_size + (remainder > 0 ? 1 : 0));
            group_world_size = group_size + (remainder > 1 ? 1 : 0);
        } else {
            // Group 2: compute z2
            group_id = 2;
            group_rank = rank - (2 * group_size + (remainder > 1 ? 2 : remainder > 0 ? 1 : 0));
            group_world_size = group_size + (remainder > 2 ? 1 : 0);
        }
        
        Polynomial z0, z1, z2;
        
        if (group_id == 0) {
            z0 = karatsuba_seq_impl(a_low, b_low, threshold);
        } else if (group_id == 1) {
            z1 = karatsuba_seq_impl(a_sum, b_sum, threshold);
        } else {
            z2 = karatsuba_seq_impl(a_high, b_high, threshold);
        }
        
        // Broadcast results to all processes
        if (group_id != 0) z0 = Polynomial({0});
        if (group_id != 1) z1 = Polynomial({0});
        if (group_id != 2) z2 = Polynomial({0});
        
        // Find representative ranks for each group
        int repr_0 = 0;
        int repr_1 = group_size + (remainder > 0 ? 1 : 0);
        int repr_2 = 2 * group_size + (remainder > 1 ? 2 : remainder > 0 ? 1 : 0);
        
        // Broadcast from representatives
        if (rank == repr_0 || group_id == 0) {
            z0.broadcast(repr_0, MPI_COMM_WORLD);
        } else {
            Polynomial temp;
            temp.broadcast(repr_0, MPI_COMM_WORLD);
            z0 = temp;
        }
        
        if (rank == repr_1 || group_id == 1) {
            z1.broadcast(repr_1, MPI_COMM_WORLD);
        } else {
            Polynomial temp;
            temp.broadcast(repr_1, MPI_COMM_WORLD);
            z1 = temp;
        }
        
        if (rank == repr_2 || group_id == 2) {
            z2.broadcast(repr_2, MPI_COMM_WORLD);
        } else {
            Polynomial temp;
            temp.broadcast(repr_2, MPI_COMM_WORLD);
            z2 = temp;
        }
        
        // Combine results
        Polynomial middle = z1.subtract(z0).subtract(z2);
        return z0.add(middle.shift(mid)).add(z2.shift(2 * mid));
        
    } else {
        // Not enough processes, compute sequentially on each
        Polynomial z0 = karatsuba_seq_impl(a_low, b_low, threshold);
        Polynomial z2 = karatsuba_seq_impl(a_high, b_high, threshold);
        Polynomial z1 = karatsuba_seq_impl(a_sum, b_sum, threshold)
            .subtract(z0)
            .subtract(z2);
        
        return z0.add(z1.shift(mid)).add(z2.shift(2 * mid));
    }
}

} // namespace multiplier
