# Magma Implementation

This project provides an implementation of the Magma encryption algorithm, including utilities for testing and benchmarking the algorithm.

## Table of Contents

- [Introduction](#introduction)
- [Prerequisites](#prerequisites)
- [Building the Project](#building-the-project)
- [Usage](#usage)
  - [Testing](#testing)
  - [Benchmarking](#benchmarking)
  - [Using the Library](#using-the-library)
- [License](#license)

## Introduction

Magma is a block cipher that provides strong encryption. This project includes an implementation of the Magma cipher in C++, along with various tests and benchmarks to evaluate its performance.

## Prerequisites

To build and run this project, you need:

- CMake (version 3.10 or higher)
- A C++11 compatible compiler (e.g., GCC, Clang, MSVC)
- C++ standard library
- Git (optional, for cloning the repository)

```bash
sudo apt-get install cmake g++ git
```

## Building the Project

1. **Clone the repository:**

   ```bash
   git clone https://github.com/KienHoSD/Magma-implement.git
   cd Magma-implement
   ```

2. **Create a build directory:**

   ```bash
   mkdir build
   cd build
   ```

3. **Run CMake to configure the project:**

   ```bash
   cmake ..
   ```

4. **Build the project:**

   ```bash
   make
   ```

   This will build the library and the executables for tests and benchmarks.

## Usage

### Testing

To test the implementation, ensure you have built the project and then run:

```bash
make run_tests
```

Or directly run individual test executables if needed:

```bash
./test_magma
./benchmark_magma
./testspeed_singlethread_magma
./testspeed_multithread_magma
```

### Benchmarking

To benchmark the implementation, run:

```bash
make run_benchmarks
```

This will execute the benchmark programs and output their performance metrics.

### Using the Library

To use the `magma_lib` library in your own project, follow these steps:

1. **Link Against the Library:**
   Include the library in your `CMakeLists.txt` by adding the following:

   ```cmake
   find_library(MAGMA_LIB NAMES magma_lib PATHS /path/to/your/build/directory)
   target_link_libraries(your_target PRIVATE ${MAGMA_LIB})
   ```

   Replace `/path/to/your/build/directory` with the path to where the library is built.

2. **Include the Header Files:**
   In your source code, include the necessary headers:

   ```cpp
   #include "magma.hpp"
   ```

3. **Instantiate and Use the Library:**

   Create an instance of the `Magma` class and use its methods to perform encryption and decryption:

   ```cpp
   #include "magma.hpp"
   #include <vector>
   #include <iostream>

   int main() {
       std::string key = "c54891ee9707aabbd126c79fd47ebb2279ef00473776b219ca3bcd8d06f8cce1";
       Magma magmacipher(key);

       std::vector<uint8_t> plaintext = {/* your data here */};
       std::vector<uint8_t> ciphertext(plaintext.size());

       magmacipher.encrypt(plaintext, ciphertext);

       std::vector<uint8_t> decrypted(plaintext.size());
       magmacipher.decrypt(ciphertext, decrypted);

       // Output results
       std::cout << "Encryption and decryption completed." << std::endl;
       return 0;
   }
   ```

   Ensure that you have linked against `magma_lib` and included the correct headers in your project.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Reference

- [GOST 28147-89](https://en.wikipedia.org/wiki/GOST_(block_cipher))

