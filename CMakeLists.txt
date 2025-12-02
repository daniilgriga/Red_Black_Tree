cmake_minimum_required (VERSION 3.12)

project (Tree LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set (COMMON_COMPILE_OPTIONS
    $<$<CXX_COMPILER_ID:GNU,Clang>: -Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>: /W4 /WX>)

add_executable (tree src/driver.cpp)
target_include_directories (tree PRIVATE include)
target_compile_options (tree PRIVATE ${COMMON_COMPILE_OPTIONS})

enable_testing()

add_executable (tree_tests tests/unit/unit_tests.cpp)
target_include_directories (tree_tests PRIVATE include)
target_compile_options (tree_tests PRIVATE ${COMMON_COMPILE_OPTIONS})

find_package (GTest REQUIRED)
target_link_libraries (tree_tests PRIVATE GTest::gtest GTest::gtest_main)

include (GoogleTest)
gtest_discover_tests (tree_tests)
