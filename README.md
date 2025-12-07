![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)

# Red Black Tree

![rb_tree_example](img/tree.png)

## General Information

### Red-Black Tree Definition

A **Red-Black Tree** is a self-balancing binary search tree that maintains specific properties to ensure efficient operations. It is characterized by the following invariants:

1. **Node Coloring**: Every node is either **red** or **black**
2. **Root Property**: The root node is always **black**
3. **Leaf Property**: All leaves (NIL nodes) are considered **black**
4. **Red Property**: No two consecutive red nodes can appear in any path (a red node cannot have red children)
5. **Black Height Property**: Every path from a given node to any of its descendant NIL nodes must contain the same number of black nodes

### Time Complexity

| Operation | Average Case | Worst Case |
|-----------|--------------|------------|
|  Search   |  O(log n)    |  O(log n)  |
|  Insert   |  O(log n)    |  O(log n)  |
|  Delete   |  O(log n)    |  O(log n)  |
|  Space    |  O(n)        |  O(n)      |

This implementation provides a complete Red-Black Tree with iterator support and range query capabilities.

## Requirements

- **CMake** >= 3.15
- **C++20** compatible compiler (GCC, Clang, MSVC)
- **GoogleTest**

## How To Install

```bash
git clone https://github.com/daniilgriga/Red_Black_Tree.git
cd Red_Black_Tree/
```

## Project Structure

```bash
Red_Black_Tree/
├── CMakeLists.txt
├── include/
│   ├── rbtree.hpp                # Red-Black Tree Impl.
│   └── processor.hpp             # Command processor
├── src/
│   ├── driver.cpp                # Main application
│   └── benchmark.cpp             # Performance benchmarks
├── tests/
│   ├── unit/
│   │   └── unit_tests.cpp        # Unit tests (GoogleTest)
│   ├── end2end/
│   │   ├── 001.dat ... 023.dat   # Test inputs
│   │   ├── 001.ans ... 023.ans   # Expected outputs
│   │   ├── gen_tests.py          # Test generator
│   │   └── run_e2e.sh            # E2E test runner
│   └── perf/
│       └── run_perf.sh           # Benchmark runner
└── img/
    └── tree.png
```

## Build Configurations

### 1. Debug Build (Development)

```bash
cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build/debug

# Run app
./build/debug/rbtree
```

### 2. Release Build (Production)

```bash
cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release
cmake --build build/release

# Run app
./build/release/rbtree
```

### 3. Debug with AddressSanitizer (Memory Checks)

To detect memory leaks and undefined behavior:

```bash
cmake -S . -B build/asan -DCMAKE_BUILD_TYPE=Debug -DENABLE_ASAN=ON
cmake --build build/asan

# Run with sanitizer
./build/asan/rbtree
```

### 4. Benchmarks Only

```bash
cmake -S . -B build/bench -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=OFF -DBUILD_BENCHMARKS=ON
cmake --build build/bench
```

## Usage

### Input Format

The program accepts commands through standard input in a single line:

#### Commands

- **`k <value>`** - Insert a key into the tree
  - `value` - integer value (key to insert)
  - Example: `k 10` - inserts the number 10

- **`q <low> <high>`** - Execute a range query
  - `low` - lower bound of the range
  - `high` - upper bound of the range
  - Returns: count of elements in range [low, high]
  - Example: `q 5 15` - counts elements in [5, 15]

#### Rules

1. **All keys are unique** - duplicate insertions are ignored
2. **Range boundaries**: `low` ≤ `high` (if `low` > `high`, returns 0)
3. **Inclusive range**: `[low, high]` - including boundary values
4. **Command order**: arbitrary, processed sequentially
5. **Unknown commands** are ignored

#### Example

**Input:**

```bash
k 10 k 20 q 8 31 q 6 9 k 30 k 40 q 15 40
```

**Output:**

```bash
2 0 3
```

**Explanation:**
- `k 10 k 20` → inserts 10, 20
- `q 8 31` → elements in [8, 31]: {10, 20} → **2**
- `q 6 9` → elements in [6, 9]: {} → **0**
- `k 30 k 40` → inserts 30, 40
- `q 15 40` → elements in [15, 40]: {20, 30, 40} → **3**

## Testing

### Unit Tests

Run GoogleTest:

```bash
# Build with tests
cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build/debug

# Run all tests
cd build/debug
ctest --output-on-failure

# Or run directly
./rbtree_tests
```

### End-to-End Tests

```bash
cd tests/end2end
./run_e2e.sh ../../build/release/rbtree
```

**Test coverage:**
- Tests **001-010:** Basic cases (small inputs)
- Tests **011-015:** Medium random cases (100-500 operations)
- Tests **016-018:** Large cases (1000-5000 operations)
- Tests **019-020:** Stress tests (10K operations)
- Tests **021-023:** Extreme tests (50K-200K operations)

### Generate New E2E Tests

```bash
cd tests/end2end
python3 gen_tests.py
```

## Performance Benchmarks

### Running Benchmarks

Compare performance of `rb::Tree` vs `std::set`:

```bash
# Build benchmarks
cmake -S . -B build/bench -DCMAKE_BUILD_TYPE=Release -DBUILD_BENCHMARKS=ON
cmake --build build/bench

# Run benchmark comparison on e2e tests
cmake --build build/bench --target perf

# Or separately with urs input
./build/bench/rbtree_bench

./build/bench/stdset_bench
```

### Benchmark Output Example (perf CMake target)

```bash
Performance Benchmark: rb::Tree vs std::set
========================================================
Test           rb::Tree     std::set      Ratio
--------------------------------------------------------
001                 2 μs          4 μs      0.50x
002                 3 μs          5 μs      0.60x
003                 2 μs          2 μs      1.00x
...                 ...           ...        ...
020              6407 μs       6107 μs      1.05x
021            194444 μs     213550 μs      0.91x
022            799722 μs     907568 μs      0.88x
023           7945358 μs    8183133 μs      0.97x
========================================================
Legend: < 1.2x = Excellent | 1.2-2.0x = Good | > 2.0x = Slow
```

### What Benchmarks Measure

- **Insert operations**: `tree.insert (key)`
- **Range queries**: `tree.range_queries_solve (low, high)`
- **Comparison**: My implementation vs C++ standard library `std::set`
