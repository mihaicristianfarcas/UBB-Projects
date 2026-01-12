#import "metal_multiplier.hpp"
#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
#import <algorithm>
#import <iostream>
#import <vector>

namespace {

// Global Metal state
id<MTLDevice> g_device = nil;
id<MTLCommandQueue> g_command_queue = nil;
id<MTLLibrary> g_library = nil;

// Compute pipeline states for each kernel
id<MTLComputePipelineState> g_naive_multiply_pipeline = nil;
id<MTLComputePipelineState> g_naive_multiply_chunked_pipeline = nil;
id<MTLComputePipelineState> g_elementwise_add_pipeline = nil;
id<MTLComputePipelineState> g_elementwise_subtract_pipeline = nil;
id<MTLComputePipelineState> g_karatsuba_combine_pipeline = nil;

bool g_initialized = false;

// Helper to create a buffer from a vector
id<MTLBuffer> create_buffer(const std::vector<long long>& data) {
    return [g_device newBufferWithBytes:data.data()
                                 length:data.size() * sizeof(long long)
                                options:MTLResourceStorageModeShared];
}

id<MTLBuffer> create_buffer(const std::vector<uint32_t>& data) {
    return [g_device newBufferWithBytes:data.data()
                                 length:data.size() * sizeof(uint32_t)
                                options:MTLResourceStorageModeShared];
}

id<MTLBuffer> create_empty_buffer(size_t count) {
    return [g_device newBufferWithLength:count * sizeof(long long)
                                 options:MTLResourceStorageModeShared];
}

// Copy buffer contents to vector
std::vector<long long> buffer_to_vector(id<MTLBuffer> buffer, size_t count) {
    std::vector<long long> result(count);
    memcpy(result.data(), [buffer contents], count * sizeof(long long));
    return result;
}

// Create a compute pipeline for a kernel function
id<MTLComputePipelineState> create_pipeline(NSString* function_name) {
    NSError* error = nil;
    id<MTLFunction> function = [g_library newFunctionWithName:function_name];
    if (!function) {
        std::cerr << "Failed to find Metal function: " << [function_name UTF8String] << std::endl;
        return nil;
    }
    
    id<MTLComputePipelineState> pipeline = [g_device newComputePipelineStateWithFunction:function error:&error];
    if (!pipeline) {
        std::cerr << "Failed to create pipeline for " << [function_name UTF8String] << ": " 
                  << [[error localizedDescription] UTF8String] << std::endl;
        return nil;
    }
    return pipeline;
}

// Perform naive multiplication on GPU
std::vector<long long> naive_multiply_gpu_impl(const std::vector<long long>& a, 
                                                const std::vector<long long>& b) {
    size_t size_a = a.size();
    size_t size_b = b.size();
    size_t result_size = size_a + size_b - 1;
    
    // Create buffers
    id<MTLBuffer> buffer_a = create_buffer(a);
    id<MTLBuffer> buffer_b = create_buffer(b);
    id<MTLBuffer> buffer_result = create_empty_buffer(result_size);
    
    std::vector<uint32_t> sizes = {
        static_cast<uint32_t>(size_a),
        static_cast<uint32_t>(size_b),
        static_cast<uint32_t>(result_size)
    };
    id<MTLBuffer> buffer_sizes = create_buffer(sizes);
    
    // Create command buffer and encoder
    id<MTLCommandBuffer> command_buffer = [g_command_queue commandBuffer];
    id<MTLComputeCommandEncoder> encoder = [command_buffer computeCommandEncoder];
    
    // Set pipeline and buffers
    [encoder setComputePipelineState:g_naive_multiply_pipeline];
    [encoder setBuffer:buffer_a offset:0 atIndex:0];
    [encoder setBuffer:buffer_b offset:0 atIndex:1];
    [encoder setBuffer:buffer_result offset:0 atIndex:2];
    [encoder setBuffer:buffer_sizes offset:0 atIndex:3];
    
    // Calculate thread configuration
    NSUInteger max_threads = [g_naive_multiply_pipeline maxTotalThreadsPerThreadgroup];
    
    MTLSize grid_size = MTLSizeMake(result_size, 1, 1);
    MTLSize threadgroup_size = MTLSizeMake(std::min(max_threads, result_size), 1, 1);
    
    [encoder dispatchThreads:grid_size threadsPerThreadgroup:threadgroup_size];
    [encoder endEncoding];
    
    // Execute and wait
    [command_buffer commit];
    [command_buffer waitUntilCompleted];
    
    // Read results
    return buffer_to_vector(buffer_result, result_size);
}

// GPU element-wise addition
std::vector<long long> add_gpu_impl(const std::vector<long long>& a, 
                                     const std::vector<long long>& b) {
    size_t size_a = a.size();
    size_t size_b = b.size();
    size_t result_size = std::max(size_a, size_b);
    
    id<MTLBuffer> buffer_a = create_buffer(a);
    id<MTLBuffer> buffer_b = create_buffer(b);
    id<MTLBuffer> buffer_result = create_empty_buffer(result_size);
    
    std::vector<uint32_t> sizes = {
        static_cast<uint32_t>(size_a),
        static_cast<uint32_t>(size_b),
        static_cast<uint32_t>(result_size)
    };
    id<MTLBuffer> buffer_sizes = create_buffer(sizes);
    
    id<MTLCommandBuffer> command_buffer = [g_command_queue commandBuffer];
    id<MTLComputeCommandEncoder> encoder = [command_buffer computeCommandEncoder];
    
    [encoder setComputePipelineState:g_elementwise_add_pipeline];
    [encoder setBuffer:buffer_a offset:0 atIndex:0];
    [encoder setBuffer:buffer_b offset:0 atIndex:1];
    [encoder setBuffer:buffer_result offset:0 atIndex:2];
    [encoder setBuffer:buffer_sizes offset:0 atIndex:3];
    
    NSUInteger max_threads = [g_elementwise_add_pipeline maxTotalThreadsPerThreadgroup];
    MTLSize grid_size = MTLSizeMake(result_size, 1, 1);
    MTLSize threadgroup_size = MTLSizeMake(std::min(max_threads, result_size), 1, 1);
    
    [encoder dispatchThreads:grid_size threadsPerThreadgroup:threadgroup_size];
    [encoder endEncoding];
    
    [command_buffer commit];
    [command_buffer waitUntilCompleted];
    
    return buffer_to_vector(buffer_result, result_size);
}

// GPU element-wise subtraction
std::vector<long long> subtract_gpu_impl(const std::vector<long long>& a, 
                                          const std::vector<long long>& b) {
    size_t size_a = a.size();
    size_t size_b = b.size();
    size_t result_size = std::max(size_a, size_b);
    
    id<MTLBuffer> buffer_a = create_buffer(a);
    id<MTLBuffer> buffer_b = create_buffer(b);
    id<MTLBuffer> buffer_result = create_empty_buffer(result_size);
    
    std::vector<uint32_t> sizes = {
        static_cast<uint32_t>(size_a),
        static_cast<uint32_t>(size_b),
        static_cast<uint32_t>(result_size)
    };
    id<MTLBuffer> buffer_sizes = create_buffer(sizes);
    
    id<MTLCommandBuffer> command_buffer = [g_command_queue commandBuffer];
    id<MTLComputeCommandEncoder> encoder = [command_buffer computeCommandEncoder];
    
    [encoder setComputePipelineState:g_elementwise_subtract_pipeline];
    [encoder setBuffer:buffer_a offset:0 atIndex:0];
    [encoder setBuffer:buffer_b offset:0 atIndex:1];
    [encoder setBuffer:buffer_result offset:0 atIndex:2];
    [encoder setBuffer:buffer_sizes offset:0 atIndex:3];
    
    NSUInteger max_threads = [g_elementwise_subtract_pipeline maxTotalThreadsPerThreadgroup];
    MTLSize grid_size = MTLSizeMake(result_size, 1, 1);
    MTLSize threadgroup_size = MTLSizeMake(std::min(max_threads, result_size), 1, 1);
    
    [encoder dispatchThreads:grid_size threadsPerThreadgroup:threadgroup_size];
    [encoder endEncoding];
    
    [command_buffer commit];
    [command_buffer waitUntilCompleted];
    
    return buffer_to_vector(buffer_result, result_size);
}

// GPU Karatsuba combine operation
std::vector<long long> karatsuba_combine_gpu_impl(const std::vector<long long>& z0,
                                                   const std::vector<long long>& z1,
                                                   const std::vector<long long>& z2,
                                                   size_t mid,
                                                   size_t result_size) {
    id<MTLBuffer> buffer_z0 = create_buffer(z0);
    id<MTLBuffer> buffer_z1 = create_buffer(z1);
    id<MTLBuffer> buffer_z2 = create_buffer(z2);
    id<MTLBuffer> buffer_result = create_empty_buffer(result_size);
    
    std::vector<uint32_t> params = {
        static_cast<uint32_t>(z0.size()),
        static_cast<uint32_t>(z1.size()),
        static_cast<uint32_t>(z2.size()),
        static_cast<uint32_t>(mid),
        static_cast<uint32_t>(result_size)
    };
    id<MTLBuffer> buffer_params = create_buffer(params);
    
    id<MTLCommandBuffer> command_buffer = [g_command_queue commandBuffer];
    id<MTLComputeCommandEncoder> encoder = [command_buffer computeCommandEncoder];
    
    [encoder setComputePipelineState:g_karatsuba_combine_pipeline];
    [encoder setBuffer:buffer_z0 offset:0 atIndex:0];
    [encoder setBuffer:buffer_z1 offset:0 atIndex:1];
    [encoder setBuffer:buffer_z2 offset:0 atIndex:2];
    [encoder setBuffer:buffer_result offset:0 atIndex:3];
    [encoder setBuffer:buffer_params offset:0 atIndex:4];
    
    NSUInteger max_threads = [g_karatsuba_combine_pipeline maxTotalThreadsPerThreadgroup];
    MTLSize grid_size = MTLSizeMake(result_size, 1, 1);
    MTLSize threadgroup_size = MTLSizeMake(std::min(max_threads, result_size), 1, 1);
    
    [encoder dispatchThreads:grid_size threadsPerThreadgroup:threadgroup_size];
    [encoder endEncoding];
    
    [command_buffer commit];
    [command_buffer waitUntilCompleted];
    
    return buffer_to_vector(buffer_result, result_size);
}

// Normalize a coefficient vector (remove trailing zeros)
void normalize(std::vector<long long>& coeffs) {
    while (coeffs.size() > 1 && coeffs.back() == 0) {
        coeffs.pop_back();
    }
}

// Recursive Karatsuba implementation using GPU for base operations
std::vector<long long> karatsuba_gpu_impl(const std::vector<long long>& a, 
                                           const std::vector<long long>& b, 
                                           int threshold) {
    size_t size = std::max(a.size(), b.size());
    
    // Base case: use GPU naive multiplication
    if (size <= static_cast<size_t>(threshold)) {
        return naive_multiply_gpu_impl(a, b);
    }
    
    size_t mid = size / 2;
    
    // Split polynomials
    std::vector<long long> a_low(a.begin(), a.begin() + std::min(mid, a.size()));
    std::vector<long long> a_high;
    if (a.size() > mid) {
        a_high.assign(a.begin() + mid, a.end());
    } else {
        a_high = {0};
    }
    
    std::vector<long long> b_low(b.begin(), b.begin() + std::min(mid, b.size()));
    std::vector<long long> b_high;
    if (b.size() > mid) {
        b_high.assign(b.begin() + mid, b.end());
    } else {
        b_high = {0};
    }
    
    // Ensure non-empty
    if (a_low.empty()) a_low = {0};
    if (a_high.empty()) a_high = {0};
    if (b_low.empty()) b_low = {0};
    if (b_high.empty()) b_high = {0};
    
    // Compute z0 = a_low * b_low
    std::vector<long long> z0 = karatsuba_gpu_impl(a_low, b_low, threshold);
    
    // Compute z2 = a_high * b_high
    std::vector<long long> z2 = karatsuba_gpu_impl(a_high, b_high, threshold);
    
    // Compute a_sum = a_low + a_high, b_sum = b_low + b_high using GPU
    std::vector<long long> a_sum = add_gpu_impl(a_low, a_high);
    std::vector<long long> b_sum = add_gpu_impl(b_low, b_high);
    
    // Compute z1_full = a_sum * b_sum
    std::vector<long long> z1_full = karatsuba_gpu_impl(a_sum, b_sum, threshold);
    
    // Compute z1 = z1_full - z0 - z2 using GPU
    std::vector<long long> z1_temp = subtract_gpu_impl(z1_full, z0);
    std::vector<long long> z1 = subtract_gpu_impl(z1_temp, z2);
    
    // Compute result size and combine using GPU
    size_t result_size = a.size() + b.size() - 1;
    std::vector<long long> result = karatsuba_combine_gpu_impl(z0, z1, z2, mid, result_size);
    
    normalize(result);
    return result;
}

} // anonymous namespace

namespace metal_multiplier {

bool initialize() {
    @autoreleasepool {
        if (g_initialized) {
            return true;
        }
        
        // Get the default Metal device
        g_device = MTLCreateSystemDefaultDevice();
        if (!g_device) {
            std::cerr << "Metal is not supported on this device" << std::endl;
            return false;
        }
        
        // Create command queue
        g_command_queue = [g_device newCommandQueue];
        if (!g_command_queue) {
            std::cerr << "Failed to create Metal command queue" << std::endl;
            return false;
        }
        
        // Load the Metal shader library
        // First try to load from the default library (embedded in app)
        NSError* error = nil;
        g_library = [g_device newDefaultLibrary];
        
        // If that fails, try to load from a metallib file
        if (!g_library) {
            // Try loading from source file
            NSString* shader_path = @"shaders/polynomial.metal";
            NSString* source = [NSString stringWithContentsOfFile:shader_path 
                                                         encoding:NSUTF8StringEncoding 
                                                            error:&error];
            if (!source) {
                // Try relative to executable
                NSBundle* bundle = [NSBundle mainBundle];
                shader_path = [bundle pathForResource:@"polynomial" ofType:@"metal"];
                if (shader_path) {
                    source = [NSString stringWithContentsOfFile:shader_path 
                                                       encoding:NSUTF8StringEncoding 
                                                          error:&error];
                }
            }
            
            if (!source) {
                // Try current directory
                shader_path = [[NSFileManager defaultManager] currentDirectoryPath];
                shader_path = [shader_path stringByAppendingPathComponent:@"shaders/polynomial.metal"];
                source = [NSString stringWithContentsOfFile:shader_path 
                                                   encoding:NSUTF8StringEncoding 
                                                      error:&error];
            }
            
            if (source) {
                MTLCompileOptions* options = [[MTLCompileOptions alloc] init];
                g_library = [g_device newLibraryWithSource:source options:options error:&error];
            }
            
            if (!g_library) {
                std::cerr << "Failed to load Metal library: " 
                          << (error ? [[error localizedDescription] UTF8String] : "unknown error") 
                          << std::endl;
                return false;
            }
        }
        
        // Create compute pipelines for each kernel
        g_naive_multiply_pipeline = create_pipeline(@"naive_multiply");
        g_naive_multiply_chunked_pipeline = create_pipeline(@"naive_multiply_chunked");
        g_elementwise_add_pipeline = create_pipeline(@"elementwise_add");
        g_elementwise_subtract_pipeline = create_pipeline(@"elementwise_subtract");
        g_karatsuba_combine_pipeline = create_pipeline(@"karatsuba_combine");
        
        if (!g_naive_multiply_pipeline || !g_elementwise_add_pipeline || 
            !g_elementwise_subtract_pipeline || !g_karatsuba_combine_pipeline) {
            std::cerr << "Failed to create one or more compute pipelines" << std::endl;
            return false;
        }
        
        g_initialized = true;
        return true;
    }
}

void cleanup() {
    @autoreleasepool {
        g_naive_multiply_pipeline = nil;
        g_naive_multiply_chunked_pipeline = nil;
        g_elementwise_add_pipeline = nil;
        g_elementwise_subtract_pipeline = nil;
        g_karatsuba_combine_pipeline = nil;
        g_library = nil;
        g_command_queue = nil;
        g_device = nil;
        g_initialized = false;
    }
}

bool is_available() {
    return g_initialized && g_device != nil;
}

std::string get_device_name() {
    if (!g_device) {
        return "No Metal device";
    }
    return std::string([[g_device name] UTF8String]);
}

Polynomial naive_gpu(const Polynomial& a, const Polynomial& b) {
    @autoreleasepool {
        if (!is_available()) {
            throw std::runtime_error("Metal not initialized. Call initialize() first.");
        }
        
        std::vector<long long> result = naive_multiply_gpu_impl(
            a.coefficients(), 
            b.coefficients()
        );
        
        return Polynomial(std::move(result));
    }
}

Polynomial karatsuba_gpu(const Polynomial& a, const Polynomial& b, int threshold) {
    @autoreleasepool {
        if (!is_available()) {
            throw std::runtime_error("Metal not initialized. Call initialize() first.");
        }
        
        std::vector<long long> result = karatsuba_gpu_impl(
            a.coefficients(), 
            b.coefficients(),
            threshold
        );
        
        return Polynomial(std::move(result));
    }
}

} // namespace metal_multiplier
