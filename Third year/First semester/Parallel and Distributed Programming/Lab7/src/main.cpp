#include "multiplier.hpp"
#include "polynomial.hpp"
#include <mpi.h>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

namespace {
struct Config {
    int degree = 500;
    int repetitions = 3;
    int bound = 50;
    int threshold = 64;
    unsigned int seed = 42;
};

Config parse_args(int argc, char** argv) {
    Config config;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        auto next = [&]() -> std::string {
            if (i + 1 >= argc) {
                throw std::runtime_error("Missing value for " + arg);
            }
            return argv[++i];
        };
        if (arg == "--degree") config.degree = std::stoi(next());
        else if (arg == "--repetitions") config.repetitions = std::stoi(next());
        else if (arg == "--bound") config.bound = std::stoi(next());
        else if (arg == "--threshold") config.threshold = std::stoi(next());
        else if (arg == "--seed") config.seed = static_cast<unsigned int>(std::stoul(next()));
        else {
            throw std::runtime_error("Unknown argument: " + arg);
        }
    }
    return config;
}

template <typename Func>
double time_ms(Func&& f) {
    auto start = std::chrono::high_resolution_clock::now();
    f();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}
} // namespace

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    
    try {
        Config config = parse_args(argc, argv);
        
        Polynomial p, q;
        
        // Only rank 0 generates polynomials
        if (rank == 0) {
            std::mt19937 rng(config.seed);
            p = Polynomial::random(config.degree, config.bound, rng);
            q = Polynomial::random(config.degree, config.bound, rng);
            
            std::cout << "Polynomial Multiplication with MPI\n";
            std::cout << "===================================\n";
            std::cout << "Degree: " << config.degree << "\n";
            std::cout << "Processes: " << world_size << "\n";
            std::cout << "Repetitions: " << config.repetitions << "\n";
            std::cout << "Threshold: " << config.threshold << "\n";
            std::cout << "Coefficient bound: " << config.bound << "\n\n";
        }
        
        // Broadcast polynomials to all processes
        p.broadcast(0, MPI_COMM_WORLD);
        q.broadcast(0, MPI_COMM_WORLD);
        
        std::map<std::string, Polynomial> results;
        std::map<std::string, double> timings;
        
        // Run benchmarks
        for (int i = 0; i < config.repetitions; ++i) {
            Polynomial result;
            
            // Naive Sequential (only on rank 0)
            if (rank == 0) {
                double duration = time_ms([&]() { 
                    result = multiplier::naive_sequential(p, q); 
                });
                timings["Naive Sequential"] += duration;
                results["Naive Sequential"] = result;
            }
            
            // Naive MPI
            double duration_naive_mpi = time_ms([&]() { 
                result = multiplier::naive_mpi(p, q, rank, world_size); 
            });
            
            if (rank == 0) {
                timings["Naive MPI"] += duration_naive_mpi;
                results["Naive MPI"] = result;
            }
            
            // Karatsuba Sequential (only on rank 0)
            if (rank == 0) {
                double duration = time_ms([&]() { 
                    result = multiplier::karatsuba_sequential(p, q, config.threshold); 
                });
                timings["Karatsuba Sequential"] += duration;
                results["Karatsuba Sequential"] = result;
            }
            
            // Karatsuba MPI
            double duration_karatsuba_mpi = time_ms([&]() { 
                result = multiplier::karatsuba_mpi(p, q, config.threshold, rank, world_size); 
            });
            
            if (rank == 0) {
                timings["Karatsuba MPI"] += duration_karatsuba_mpi;
                results["Karatsuba MPI"] = result;
            }
        }
        
        // Verify and print results (only rank 0)
        if (rank == 0) {
            // Verify equality of all results
            const Polynomial* reference = nullptr;
            for (const auto& [label, poly] : results) {
                if (!reference) {
                    reference = &poly;
                    continue;
                }
                if (poly.coefficients() != reference->coefficients()) {
                    std::cerr << "ERROR: Mismatch detected for " << label << "\n";
                    std::cerr << "Result degree: " << poly.degree() 
                              << " vs reference degree: " << reference->degree() << "\n";
                    MPI_Abort(MPI_COMM_WORLD, 1);
                }
            }
            
            std::cout << "All algorithms produced identical results. ✓\n\n";
            std::cout << "Average timings (ms) over " << config.repetitions << " run(s):\n";
            std::cout << "─────────────────────────────────────────────────────\n";
            
            double seq_naive_time = timings["Naive Sequential"] / config.repetitions;
            double mpi_naive_time = timings["Naive MPI"] / config.repetitions;
            double seq_karatsuba_time = timings["Karatsuba Sequential"] / config.repetitions;
            double mpi_karatsuba_time = timings["Karatsuba MPI"] / config.repetitions;
            
            std::cout << "  Naive Sequential:      " << seq_naive_time << " ms\n";
            std::cout << "  Naive MPI:             " << mpi_naive_time << " ms";
            std::cout << "  (Speedup: " << (seq_naive_time / mpi_naive_time) << "x)\n";
            
            std::cout << "  Karatsuba Sequential:  " << seq_karatsuba_time << " ms\n";
            std::cout << "  Karatsuba MPI:         " << mpi_karatsuba_time << " ms";
            std::cout << "  (Speedup: " << (seq_karatsuba_time / mpi_karatsuba_time) << "x)\n";
            
            std::cout << "\nComparison:\n";
            std::cout << "  MPI Naive vs Sequential Naive:         " 
                      << ((mpi_naive_time < seq_naive_time) ? "faster" : "slower") 
                      << " by " << std::abs(seq_naive_time - mpi_naive_time) << " ms\n";
            std::cout << "  MPI Karatsuba vs Sequential Karatsuba: " 
                      << ((mpi_karatsuba_time < seq_karatsuba_time) ? "faster" : "slower") 
                      << " by " << std::abs(seq_karatsuba_time - mpi_karatsuba_time) << " ms\n";
        }
        
    } catch (const std::exception& ex) {
        if (rank == 0) {
            std::cerr << "Error: " << ex.what() << "\n";
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    
    MPI_Finalize();
    return EXIT_SUCCESS;
}
