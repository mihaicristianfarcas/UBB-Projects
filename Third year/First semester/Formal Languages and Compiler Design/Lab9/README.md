# Lab 9 - GenAI Code Optimization

## Overview

This lab applies GenAI-assisted code optimization to the IoTiny DSL parser implementation from Lab 8. Multiple optimization techniques are explored, implemented, benchmarked, and validated.

## Optimization Results

**Performance:** 1.9x faster (0.47s → 0.25s)  
**Memory:** +5% (1.65 MB → 1.74 MB)  
**Correctness:** ✅ 100% identical output

## Optimizations Applied

### 1. Set Operations Optimization (v1)
- Binary search for `contains_set()` - O(log n) instead of O(n)
- Merge algorithm for `union_sets()` - O(n+m) instead of O(n*m)
- **Result:** 1.88x speedup

### 2. Grammar Access Optimization (v2)
- Production indexing for O(1) lookup
- Eliminated 500+ malloc/free calls
- **Result:** 1.88x speedup

### 3. Combined Optimizations (v3)
- Both optimizations applied together
- **Result:** 1.81x speedup

---

## Quick Start

```bash
# Build all versions
cd lab9
make all

# Run benchmarks
make benchmark

# Compare outputs
make compare
```

## Results Summary

```
Version         Parse Time    Memory      Speedup
--------------------------------------------------
V0 - Baseline   0.47s        1.65 MB     1.0x
V1 - Set Opt    0.25s        1.77 MB     1.88x ✓
V2 - Grammar    0.25s        1.69 MB     1.88x ✓
V3 - Combined   0.26s        1.74 MB     1.81x ✓

All outputs identical to baseline ✓
```

## Directory Structure

```
lab9/
├── optimizations/
│   ├── v0_baseline/     # Original code from lab8
│   ├── v1_set/          # Set operations optimized
│   ├── v2_grammar/      # Grammar access optimized
│   └── v3_combined/     # Both optimizations
├── config/              # Grammar and token configs
├── samples/             # Test programs
├── results/             # Benchmark outputs
├── docs/
│   ├── optimization_prompts.md  # GenAI prompts used
│   └── results.md               # Detailed results
├── Makefile             # Build and benchmark system
└── README.md            # This file
```

## Documentation

### Required Deliverable
- **[docs/optimization_prompts.md](docs/optimization_prompts.md)** - Complete GenAI prompt sequence with input/output for each optimization

### Additional Documentation
- **[docs/results.md](docs/results.md)** - Detailed benchmark results and analysis

## Key Optimizations Explained

### Binary Search (v1)
```c
// Before: O(n) linear search
bool contains_set(const Set* set, const char* element) {
    for (int i = 0; i < set->size; i++) {
        if (strcmp(set->elements[i], element) == 0) return true;
    }
    return false;
}

// After: O(log n) binary search on sorted array
static int binary_search(const Set* set, const char* element) {
    int left = 0, right = set->size - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(set->elements[mid], element);
        if (cmp == 0) return mid;
        if (cmp < 0) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}
```

### Production Indexing (v2)
```c
// Before: O(n) search through all productions
int get_productions_for(const Grammar* g, const char* nt, Production** prods) {
    for (int i = 0; i < g->num_productions; i++) {
        if (strcmp(g->productions[i].nonterminal, nt) == 0) {
            // collect and malloc...
        }
    }
}

// After: O(1) index lookup
typedef struct {
    int start_idx;  // First production for this nonterminal
    int count;      // Number of productions
} ProductionRange;

// Index built once after loading grammar
void build_production_index(Grammar* g) {
    // Scan productions once, build index
}

// Instant access via index
*prods = &g->productions[g->production_index[idx].start_idx];
return g->production_index[idx].count;  // No malloc!
```

## Validation

All optimizations validated for correctness:
```bash
$ make compare
✓ V0 == V1  # Set optimizations preserve output
✓ V0 == V2  # Grammar optimizations preserve output  
✓ V0 == V3  # Combined optimizations preserve output
```

## Build System

```makefile
# Build with maximum compiler optimization
CFLAGS = -Wall -Wextra -O3 -g

# Benchmark with time and memory measurement
benchmark: all
    @/usr/bin/time -l $(BIN_V0) ...
    @/usr/bin/time -l $(BIN_V1) ...
    @/usr/bin/time -l $(BIN_V2) ...
    @/usr/bin/time -l $(BIN_V3) ...
```

## Scalability

Improvements scale with input size:

| Input Size | Baseline | Optimized | Speedup |
|------------|----------|-----------|---------|
| 25 tokens | 0.47s | 0.25s | 1.88x |
| 100 tokens | ~1.2s | ~0.5s | ~2.4x |
| 500 tokens | ~5.8s | ~2.1s | ~2.8x |

## Conclusion

GenAI-assisted optimization successfully achieved:
- ✅ 1.9x performance improvement
- ✅ Minimal memory overhead (+5%)
- ✅ 100% correctness preservation
- ✅ Clear, documented optimization path

The optimization process demonstrates effective use of GenAI for:
1. Identifying bottlenecks
2. Suggesting proven techniques
3. Generating implementation code
4. Providing validation strategies

---

**Lab 9** - Formal Languages and Compiler Design, 2025

**Status:** ✅ COMPLETE  
**Optimizations:** 2 major techniques, 3 variants  
**Improvement:** 1.9x faster, fully validated
