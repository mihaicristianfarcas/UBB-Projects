# Polynomial Multiplication (Sequential vs Parallel)

Implements four variants for multiplying two polynomials: naive O(n²) and Karatsuba, each in sequential and parallel form. Coefficients use `long long` for simplicity (swap the alias in `polynomial.hpp` for true big-integer support if a multiprecision library is available).

## Build
```bash
mkdir -p bin
g++ -std=c++20 -O2 -Wall -Wextra -Wpedantic -pthread src/*.cpp -o bin/polymul
```

## Run
```bash
# defaults: degree 500, repetitions 3, threads = hardware_concurrency, threshold 64
./bin/polymul

# custom parameters
./bin/polymul --degree 2000 --repetitions 5 --threads 8 --threshold 128 --bound 100 --seed 123
```
Output prints the average duration (ms) per variant over the requested repetitions.

## Algorithms
- **Naive sequential**: classic O(n²) double loop.
- **Naive parallel**: split the first polynomial’s indices into chunks; each `std::async` task accumulates a partial result vector, then the main thread reduces partials element-wise. Synchronization is implicit via futures (no shared writes across threads).
- **Karatsuba sequential**: recursive divide-and-conquer; stop at `threshold` and fall back to naive.
- **Karatsuba parallel**: the two independent subproducts (`z0`, `z2`) run via `std::async` while the cross term is computed in the current thread. A depth cap (`max_depth ≈ log2(threads)`) avoids oversubscription; below that depth it reverts to the sequential Karatsuba helper.

## Performance Measurement
- Use the `--repetitions` flag to average out noise.
- Increase `--threshold` to reduce Karatsuba overhead on smaller degrees; decrease it for very large degrees.
- Compare `--threads 1` vs multi-core runs to observe parallel speedup.

## Notes and Limitations
- Coefficients are `long long`; extremely large degrees or bounds may overflow. For the bonus big-number variant, replace `using coeff_t = long long;` in `polynomial.hpp` with a multiprecision type (e.g., `boost::multiprecision::cpp_int`) and rebuild.
- Random coefficient magnitude is controlled by `--bound` (values are uniform in `[1, bound]` with random sign).
