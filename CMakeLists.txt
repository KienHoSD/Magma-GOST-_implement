# CMake minimum version
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Magma_implement VERSION 1.0)

# Set C++ standard (assuming C++11 or later is required)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the library target for the library
add_library(magma_lib
    src/magma_utils.cpp
    src/magma_base.cpp
    src/magma_ecb.cpp
    src/magma_cbc.cpp
    src/magma.cpp
    src/pad.cpp
)

# Include header files
target_include_directories(magma_lib PRIVATE src)

# Add the executable target for the binary `test_magma`
add_executable(test_magma
    test/test_magma.cpp
)

# Add the executable target for the binary `benchmark_magma`
add_executable(benchmark_magma
    benchmark/benchmark_magma.cpp
)

# Add the executable target for the binary `testspeed_singlethread_magma`
add_executable(testspeed_singlethread_magma
    benchmark/testspeed_singlethread_magma.cpp
)

# Add the executable target for the binary `testspeed_multithread_magma`
add_executable(testspeed_multithread_magma
    benchmark/testspeed_multithread_magma.cpp
)

# Link the executable with the library
target_link_libraries(test_magma PRIVATE magma_lib)
target_link_libraries(benchmark_magma PRIVATE magma_lib)
target_link_libraries(testspeed_singlethread_magma PRIVATE magma_lib)
target_link_libraries(testspeed_multithread_magma PRIVATE magma_lib)

# Enable testing
include(CTest)

# Add tests
add_test(NAME test_magma COMMAND test_magma)

# Custom target to run all tests and print their results
add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose
    DEPENDS test_magma benchmark_magma testspeed_singlethread_magma testspeed_multithread_magma
    COMMENT "Running all tests"
)

# Custom target to run benchmarks and print their results
add_custom_target(run_benchmarks
    COMMAND testspeed_singlethread_magma 
    COMMAND testspeed_multithread_magma
    COMMAND benchmark_magma
    DEPENDS testspeed_singlethread_magma testspeed_multithread_magma benchmark_magma
    COMMENT "Running all benchmarks"
)