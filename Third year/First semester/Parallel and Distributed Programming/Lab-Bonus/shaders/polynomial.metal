#include <metal_stdlib>
using namespace metal;

/// Naive O(n²) polynomial multiplication kernel
/// Each thread computes one coefficient of the result polynomial.
/// For result[k], we sum all a[i] * b[j] where i + j == k.
kernel void naive_multiply(
    device const long* a [[buffer(0)]],           // Coefficients of polynomial A
    device const long* b [[buffer(1)]],           // Coefficients of polynomial B
    device long* result [[buffer(2)]],            // Output coefficients
    device const uint* sizes [[buffer(3)]],       // [size_a, size_b, result_size]
    uint tid [[thread_position_in_grid]]          // Thread ID = output coefficient index
) {
    uint size_a = sizes[0];
    uint size_b = sizes[1];
    uint result_size = sizes[2];
    
    if (tid >= result_size) {
        return;
    }
    
    long sum = 0;
    
    // For coefficient at position tid (k), sum a[i] * b[k-i] for valid i
    uint i_start = (tid >= size_b) ? (tid - size_b + 1) : 0;
    uint i_end = min(tid + 1, size_a);
    
    for (uint i = i_start; i < i_end; ++i) {
        uint j = tid - i;
        sum += a[i] * b[j];
    }
    
    result[tid] = sum;
}

/// Alternative naive kernel: each thread handles multiple output coefficients
/// (for cases where result_size > max threads)
kernel void naive_multiply_chunked(
    device const long* a [[buffer(0)]],
    device const long* b [[buffer(1)]],
    device long* result [[buffer(2)]],
    device const uint* sizes [[buffer(3)]],       // [size_a, size_b, result_size, chunk_start]
    uint tid [[thread_position_in_grid]],
    uint num_threads [[threads_per_grid]]
) {
    uint size_a = sizes[0];
    uint size_b = sizes[1];
    uint result_size = sizes[2];
    
    // Each thread processes multiple coefficients in strided fashion
    for (uint k = tid; k < result_size; k += num_threads) {
        long sum = 0;
        
        uint i_start = (k >= size_b) ? (k - size_b + 1) : 0;
        uint i_end = min(k + 1, size_a);
        
        for (uint i = i_start; i < i_end; ++i) {
            uint j = k - i;
            sum += a[i] * b[j];
        }
        
        result[k] = sum;
    }
}

/// Kernel for computing partial products in rows (for parallel reduction approach)
/// Each thread computes one row of the multiplication matrix: a[i] * b[0..n]
kernel void compute_partial_row(
    device const long* a [[buffer(0)]],
    device const long* b [[buffer(1)]],
    device long* partials [[buffer(2)]],          // Flattened 2D array [size_a][result_size]
    device const uint* sizes [[buffer(3)]],
    uint tid [[thread_position_in_grid]]
) {
    uint size_a = sizes[0];
    uint size_b = sizes[1];
    uint result_size = sizes[2];
    
    if (tid >= size_a) {
        return;
    }
    
    uint row_offset = tid * result_size;
    long coeff_a = a[tid];
    
    // Zero out this row first
    for (uint k = 0; k < result_size; ++k) {
        partials[row_offset + k] = 0;
    }
    
    // Compute a[tid] * b[j] for all j
    for (uint j = 0; j < size_b; ++j) {
        partials[row_offset + tid + j] = coeff_a * b[j];
    }
}

/// Reduction kernel: sum partial results into final result
kernel void reduce_partials(
    device const long* partials [[buffer(0)]],    // Flattened [num_rows][result_size]
    device long* result [[buffer(1)]],
    device const uint* sizes [[buffer(2)]],       // [num_rows, result_size]
    uint tid [[thread_position_in_grid]]
) {
    uint num_rows = sizes[0];
    uint result_size = sizes[1];
    
    if (tid >= result_size) {
        return;
    }
    
    long sum = 0;
    for (uint row = 0; row < num_rows; ++row) {
        sum += partials[row * result_size + tid];
    }
    result[tid] = sum;
}

// ============================================================================
// Karatsuba Helper Kernels
// These kernels support the Karatsuba algorithm by performing element-wise
// operations on coefficient arrays in parallel.
// ============================================================================

/// Element-wise addition: result[i] = a[i] + b[i]
kernel void elementwise_add(
    device const long* a [[buffer(0)]],
    device const long* b [[buffer(1)]],
    device long* result [[buffer(2)]],
    device const uint* sizes [[buffer(3)]],       // [size_a, size_b, result_size]
    uint tid [[thread_position_in_grid]]
) {
    uint size_a = sizes[0];
    uint size_b = sizes[1];
    uint result_size = sizes[2];
    
    if (tid >= result_size) {
        return;
    }
    
    long val_a = (tid < size_a) ? a[tid] : 0;
    long val_b = (tid < size_b) ? b[tid] : 0;
    result[tid] = val_a + val_b;
}

/// Element-wise subtraction: result[i] = a[i] - b[i]
kernel void elementwise_subtract(
    device const long* a [[buffer(0)]],
    device const long* b [[buffer(1)]],
    device long* result [[buffer(2)]],
    device const uint* sizes [[buffer(3)]],       // [size_a, size_b, result_size]
    uint tid [[thread_position_in_grid]]
) {
    uint size_a = sizes[0];
    uint size_b = sizes[1];
    uint result_size = sizes[2];
    
    if (tid >= result_size) {
        return;
    }
    
    long val_a = (tid < size_a) ? a[tid] : 0;
    long val_b = (tid < size_b) ? b[tid] : 0;
    result[tid] = val_a - val_b;
}

/// Shift kernel: result[i] = (i >= shift) ? a[i - shift] : 0
kernel void shift_polynomial(
    device const long* a [[buffer(0)]],
    device long* result [[buffer(1)]],
    device const uint* params [[buffer(2)]],      // [size_a, shift_amount, result_size]
    uint tid [[thread_position_in_grid]]
) {
    uint size_a = params[0];
    uint shift = params[1];
    uint result_size = params[2];
    
    if (tid >= result_size) {
        return;
    }
    
    if (tid < shift) {
        result[tid] = 0;
    } else {
        uint src_idx = tid - shift;
        result[tid] = (src_idx < size_a) ? a[src_idx] : 0;
    }
}

/// Combined Karatsuba recombination kernel
/// Computes: result = z0 + (z1 * x^mid) + (z2 * x^(2*mid))
/// where the shifts are implicit based on offsets
kernel void karatsuba_combine(
    device const long* z0 [[buffer(0)]],
    device const long* z1 [[buffer(1)]],
    device const long* z2 [[buffer(2)]],
    device long* result [[buffer(3)]],
    device const uint* params [[buffer(4)]],      // [size_z0, size_z1, size_z2, mid, result_size]
    uint tid [[thread_position_in_grid]]
) {
    uint size_z0 = params[0];
    uint size_z1 = params[1];
    uint size_z2 = params[2];
    uint mid = params[3];
    uint result_size = params[4];
    
    if (tid >= result_size) {
        return;
    }
    
    long sum = 0;
    
    // Add z0[tid] (no shift)
    if (tid < size_z0) {
        sum += z0[tid];
    }
    
    // Add z1[tid - mid] (shifted by mid)
    if (tid >= mid && (tid - mid) < size_z1) {
        sum += z1[tid - mid];
    }
    
    // Add z2[tid - 2*mid] (shifted by 2*mid)
    uint shift2 = 2 * mid;
    if (tid >= shift2 && (tid - shift2) < size_z2) {
        sum += z2[tid - shift2];
    }
    
    result[tid] = sum;
}

/// Copy kernel: copy a range of coefficients
kernel void copy_coefficients(
    device const long* src [[buffer(0)]],
    device long* dst [[buffer(1)]],
    device const uint* params [[buffer(2)]],      // [src_offset, dst_offset, count]
    uint tid [[thread_position_in_grid]]
) {
    uint src_offset = params[0];
    uint dst_offset = params[1];
    uint count = params[2];
    
    if (tid >= count) {
        return;
    }
    
    dst[dst_offset + tid] = src[src_offset + tid];
}
