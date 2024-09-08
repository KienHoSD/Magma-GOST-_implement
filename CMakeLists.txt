# CMake minimum version
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Magma_implement VERSION 1.0)

# Set C++ standard (assuming C++11 or later is required)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the library target for the library
add_library(magma_lib
    src/magma.cpp
    src/helper.cpp
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

# Add the executable target for the binary `testspeed_magma`
add_executable(testspeed_magma
    benchmark/testspeed_magma.cpp
)

# Link the executable with the library
target_link_libraries(test_magma PRIVATE magma_lib)
target_link_libraries(benchmark_magma PRIVATE magma_lib)
target_link_libraries(testspeed_magma PRIVATE magma_lib)