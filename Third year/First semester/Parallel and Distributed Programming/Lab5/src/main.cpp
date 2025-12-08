#include "multiplier.hpp"
#include "polynomial.hpp"

#include <chrono>
#include <cmath>
#include <cstdlib>
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
        else {
            throw std::runtime_error("Unknown argument: " + arg);
        }
    }
    return config;
}

template <typename Func>
long long time_ms(Func&& f) {
    auto start = std::chrono::high_resolution_clock::now();
    f();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}
} // namespace

int main(int argc, char** argv) {
    try {
        Config config = parse_args(argc, argv);
        std::mt19937 rng(config.seed);

        Polynomial p = Polynomial::random(config.degree, config.bound, rng);
        Polynomial q = Polynomial::random(config.degree, config.bound, rng);

        std::cout << "Degree=" << config.degree
                  << ", threads=" << config.threads
                  << ", repetitions=" << config.repetitions
                  << ", threshold=" << config.threshold
                  << "\n";

        std::map<std::string, Polynomial> results;
        std::map<std::string, long long> timings;

        int max_depth = static_cast<int>(std::max(1.0, std::floor(std::log2(std::max(1, config.threads)))));

        for (int i = 0; i < config.repetitions; ++i) {
            auto measure = [&](const std::string& label, auto&& func) {
                long long duration = time_ms([&]() { results[label] = func(); });
                timings[label] += duration;
            };

            measure("Naive Sequential", [&]() { return multiplier::naive_sequential(p, q); });
            measure("Naive Parallel", [&]() { return multiplier::naive_parallel(p, q, config.threads); });
            measure("Karatsuba Sequential", [&]() { return multiplier::karatsuba_sequential(p, q, config.threshold); });
            measure("Karatsuba Parallel", [&]() { return multiplier::karatsuba_parallel(p, q, config.threshold, max_depth); });
        }

        // verify equality of all results
        const Polynomial* reference = nullptr;
        for (const auto& [label, poly] : results) {
            if (!reference) {
                reference = &poly;
                continue;
            }
            if (poly.coefficients() != reference->coefficients()) {
                throw std::runtime_error("Mismatch detected for " + label);
            }
        }

        std::cout << "\nAverage timings (ms) over " << config.repetitions << " run(s):\n";
        for (const auto& [label, total] : timings) {
            std::cout << "  " << label << ": " << (total / config.repetitions) << "\n";
        }

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
