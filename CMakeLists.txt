cmake_minimum_required (VERSION 3.15)

project (RBTree LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

option (BUILD_TESTS "Build unit tests" ON)
option (BUILD_BENCHMARKS "Build performance benchmarks" ON)

set (COMMON_COMPILE_OPTIONS
    $<$<CXX_COMPILER_ID:GNU,Clang>: -Wall -Wextra -Wpedantic -Werror>
    $<$<CXX_COMPILER_ID:MSVC>: /W4 /WX>
)

option (ENABLE_ASAN "Enable AddressSanitizer" OFF)
if (ENABLE_ASAN)
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
    set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address")
endif ()

# main app
add_executable (rbtree src/driver.cpp)
target_include_directories (rbtree PRIVATE include)
target_compile_options (rbtree PRIVATE ${COMMON_COMPILE_OPTIONS})

# unit tests
if (BUILD_TESTS)
    enable_testing ()

    add_executable (rbtree_tests tests/unit/unit_tests.cpp)
    target_include_directories (rbtree_tests PRIVATE include)
    target_compile_options (rbtree_tests PRIVATE ${COMMON_COMPILE_OPTIONS})

    find_package (GTest REQUIRED)
    target_link_libraries (rbtree_tests PRIVATE GTest::gtest GTest::gtest_main)

    include (GoogleTest)
    gtest_discover_tests (rbtree_tests)
endif ()

# benchmarks
if (BUILD_BENCHMARKS)
    add_executable (rbtree_bench src/benchmark.cpp)
    target_include_directories (rbtree_bench PRIVATE include)
    target_compile_options (rbtree_bench PRIVATE ${COMMON_COMPILE_OPTIONS})

    # force release optimizations for benchmarks
    target_compile_options (rbtree_bench PRIVATE -O3 -march=native)

    add_custom_target (perf
        COMMAND bash ${CMAKE_SOURCE_DIR}/tests/perf/run_perf.sh ${CMAKE_BINARY_DIR}/rbtree_bench
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/perf
        DEPENDS rbtree_bench
    )
endif ()
