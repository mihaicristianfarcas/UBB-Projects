#include "cpu_multiplier.hpp"
#include "metal_multiplier.hpp"
#include "polynomial.hpp"

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <thread>
#include <utility>
#include <vector>

namespace {

struct Config {
    int degree = 500;
    int repetitions = 3;
    int threads = static_cast<int>(std::thread::hardware_concurrency());
    int bound = 50;
    int threshold = 64;
    unsigned int seed = 42;
    bool verify = true;
    bool verbose = false;
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
        else if (arg == "--threads") config.threads = std::stoi(next());
        else if (arg == "--bound") config.bound = std::stoi(next());
        else if (arg == "--threshold") config.threshold = std::stoi(next());
        else if (arg == "--seed") config.seed = static_cast<unsigned int>(std::stoul(next()));
        else if (arg == "--no-verify") config.verify = false;
        else if (arg == "--verbose" || arg == "-v") config.verbose = true;
        else if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: " << argv[0] << " [options]\n"
                      << "Options:\n"
                      << "  --degree N        Polynomial degree (default: 500)\n"
                      << "  --repetitions N   Number of repetitions (default: 3)\n"
                      << "  --threads N       Number of CPU threads (default: hardware_concurrency)\n"
                      << "  --bound N         Coefficient magnitude bound (default: 50)\n"
                      << "  --threshold N     Karatsuba threshold (default: 64)\n"
                      << "  --seed N          Random seed (default: 42)\n"
                      << "  --no-verify       Skip result verification\n"
                      << "  --verbose, -v     Verbose output\n"
                      << "  --help, -h        Show this help\n";
            std::exit(0);
        }
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

void print_separator() {
    std::cout << std::string(70, '-') << "\n";
}

} // namespace

int main(int argc, char** argv) {
    try {
        Config config = parse_args(argc, argv);
        std::mt19937 rng(config.seed);

        // Initialize Metal
        std::cout << "Initializing Metal...\n";
        if (!metal_multiplier::initialize()) {
            std::cerr << "Warning: Metal initialization failed. GPU tests will be skipped.\n";
        } else {
            std::cout << "Metal device: " << metal_multiplier::get_device_name() << "\n";
        }
        
        print_separator();

        // Generate random polynomials
        Polynomial p = Polynomial::random(config.degree, config.bound, rng);
        Polynomial q = Polynomial::random(config.degree, config.bound, rng);

        std::cout << "Configuration:\n"
                  << "  Polynomial degree: " << config.degree << "\n"
                  << "  CPU threads: " << config.threads << "\n"
                  << "  Repetitions: " << config.repetitions << "\n"
                  << "  Karatsuba threshold: " << config.threshold << "\n"
                  << "  Verification: " << (config.verify ? "enabled" : "disabled") << "\n";
        
        print_separator();

        int max_depth = static_cast<int>(std::max(1.0, std::floor(std::log2(std::max(1, config.threads)))));
        bool metal_available = metal_multiplier::is_available();

        std::map<std::string, Polynomial> results;
        std::map<std::string, double> timings;

        // Define the algorithms to benchmark
        struct Algorithm {
            std::string name;
            std::function<Polynomial()> func;
            bool enabled;
        };

        std::vector<Algorithm> algorithms = {
            {"CPU Naive Sequential", 
             [&]() { return cpu_multiplier::naive_sequential(p, q); }, 
             true},
            {"CPU Naive Parallel (" + std::to_string(config.threads) + " threads)", 
             [&]() { return cpu_multiplier::naive_parallel(p, q, config.threads); }, 
             true},
            {"CPU Karatsuba Sequential", 
             [&]() { return cpu_multiplier::karatsuba_sequential(p, q, config.threshold); }, 
             true},
            {"CPU Karatsuba Parallel", 
             [&]() { return cpu_multiplier::karatsuba_parallel(p, q, config.threshold, max_depth); }, 
             true},
            {"Metal GPU Naive", 
             [&]() { return metal_multiplier::naive_gpu(p, q); }, 
             metal_available},
            {"Metal GPU Karatsuba", 
             [&]() { return metal_multiplier::karatsuba_gpu(p, q, config.threshold); }, 
             metal_available},
        };

        std::cout << "Running benchmarks...\n\n";

        // Run benchmarks
        for (int rep = 0; rep < config.repetitions; ++rep) {
            if (config.verbose) {
                std::cout << "Repetition " << (rep + 1) << "/" << config.repetitions << ":\n";
            }

            for (auto& algo : algorithms) {
                if (!algo.enabled) continue;

                double duration = time_ms([&]() { results[algo.name] = algo.func(); });
                timings[algo.name] += duration;

                if (config.verbose) {
                    std::cout << "  " << std::left << std::setw(35) << algo.name 
                              << std::right << std::setw(10) << std::fixed << std::setprecision(2) 
                              << duration << " ms\n";
                }
            }
        }

        // Verify results
        if (config.verify) {
            std::cout << "\nVerifying results...\n";
            const Polynomial* reference = nullptr;
            std::string reference_name;
            bool all_match = true;

            for (const auto& [name, poly] : results) {
                if (!reference) {
                    reference = &poly;
                    reference_name = name;
                    continue;
                }
                if (poly.coefficients() != reference->coefficients()) {
                    std::cerr << "MISMATCH: " << name << " differs from " << reference_name << "\n";
                    all_match = false;
                }
            }

            if (all_match) {
                std::cout << "All results match!\n";
            }
        }

        print_separator();

        // Print results
        std::cout << "\nAverage timings over " << config.repetitions << " run(s):\n\n";
        std::cout << std::left << std::setw(40) << "Algorithm" 
                  << std::right << std::setw(12) << "Time (ms)"
                  << std::setw(12) << "Speedup" << "\n";
        std::cout << std::string(64, '-') << "\n";

        double baseline = timings["CPU Naive Sequential"] / config.repetitions;

        for (const auto& algo : algorithms) {
            if (!algo.enabled) continue;

            double avg_time = timings[algo.name] / config.repetitions;
            double speedup = baseline / avg_time;

            std::cout << std::left << std::setw(40) << algo.name 
                      << std::right << std::setw(12) << std::fixed << std::setprecision(2) << avg_time
                      << std::setw(11) << std::setprecision(2) << speedup << "x\n";
        }

        print_separator();

        // Print summary
        if (metal_available) {
            double cpu_naive = timings["CPU Naive Sequential"] / config.repetitions;
            double gpu_naive = timings["Metal GPU Naive"] / config.repetitions;
            double cpu_karatsuba = timings["CPU Karatsuba Sequential"] / config.repetitions;
            double gpu_karatsuba = timings["Metal GPU Karatsuba"] / config.repetitions;

            std::cout << "\nSummary:\n";
            std::cout << "  GPU Naive vs CPU Naive: " 
                      << std::fixed << std::setprecision(2) << (cpu_naive / gpu_naive) << "x speedup\n";
            std::cout << "  GPU Karatsuba vs CPU Karatsuba: " 
                      << std::fixed << std::setprecision(2) << (cpu_karatsuba / gpu_karatsuba) << "x speedup\n";
            std::cout << "  Best GPU vs Best CPU: "
                      << std::fixed << std::setprecision(2) 
                      << (std::min(cpu_naive, cpu_karatsuba) / std::min(gpu_naive, gpu_karatsuba)) 
                      << "x speedup\n";
        }

        // Cleanup
        metal_multiplier::cleanup();

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
