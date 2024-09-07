# CMake minimum version
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Magma_implement VERSION 1.0)

# Set C++ standard (assuming C++11 or later is required)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable target for the binary `magma`
add_executable(magma
    src/magma.cpp
    src/helper.cpp
    src/pad.cpp
    src/main.cpp
)

# Include header files
target_include_directories(magma PRIVATE src)
