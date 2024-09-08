#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <ctime>
#include <cstring>
#include <random> // For std::mt19937 and std::uniform_int_distribution
#include <vector>
#include "../src/magma.hpp"

#ifdef _WIN32
std::ofstream results_encrypt("benchmark_results_enc_win.csv");
std::ofstream results_decrypt("benchmark_results_dec_win.csv");
#else
std::ofstream results_encrypt("benchmark_results_enc_linux.csv");
std::ofstream results_decrypt("benchmark_results_dec_linux.csv");
#endif

// File sizes to test: 2 KB, 10 KB, 20 KB, 50 KB, 100 KB, 500 KB, 2 MB
std::vector<int> fileSizes = {2 * 1024, 10 * 1024, 20 * 1024, 50 * 1024, 100 * 1024, 500 * 1024, 2000 * 1024};
const unsigned int MEGABYTE = 1024 * 1024;
const unsigned int MIN_SIZE = 1024;         // 1 KB
const unsigned int MAX_SIZE = 2 * MEGABYTE; // 2 MB
const unsigned int NUM_TESTS = 10000;       // Number of different random sizes to test
const unsigned int MICROSECONDS = 1000000;

unsigned long getMicroseconds()
{
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  return MICROSECONDS * tv.tv_sec + tv.tv_usec;
}

unsigned long getMilliseconds()
{
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  return 1000 * tv.tv_sec + tv.tv_usec / 1000;
}

unsigned char *getRandomPlain(unsigned int length)
{
  unsigned char *plain = new unsigned char[length];
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 255); // Distribution for random byte values
  for (unsigned int i = 0; i < length; i++)
  {
    plain[i] = static_cast<unsigned char>(dis(gen));
  }
  return plain;
}

void TestEncryptDecrypt(int times, int size, std::ofstream &results_encrypt, std::ofstream &results_decrypt)
{

  // Constant key
  std::string key = "c54891ee9707aabbd126c79fd47ebb2279ef00473776b219ca3bcd8d06f8cce1";
  Magma magmacipher(key);

  for (int i = 0; i < times; ++i)
  {
    std::vector<uint8_t> plain(size);
    std::vector<uint8_t> out(size);
    std::vector<uint8_t> recovered(size);

    unsigned long start_enc = getMicroseconds();
    magmacipher.encrypt(plain, out);
    unsigned long delta_enc = getMicroseconds() - start_enc;

    unsigned long start_dec = getMicroseconds();
    magmacipher.decrypt(out, recovered);
    unsigned long delta_dec = getMicroseconds() - start_dec;

    // check
    if (std::memcmp(plain.data(), recovered.data(), size) != 0)
    {
      std::cerr << "Decryption failed!" << std::endl;
      return;
    }

    results_encrypt << size << "," << delta_enc << "\n";
    results_decrypt << size << "," << delta_dec << "\n";
  }
}

int main()
{
  results_encrypt << "Size(Bytes),Time(Microseconds)\n";
  results_decrypt << "Size(Bytes),Time(Microseconds)\n";

  std::cout << "Start benchmark" << std::endl;
  std::cout << "Testing encrypt, decrypt with 7 different file sizes: 2 KB, 10 KB, 20 KB, 50 KB, 100 KB, 500 KB, 2 MB" << std::endl;

  for (auto filesize : fileSizes)
  {
    unsigned long start = getMilliseconds();
    TestEncryptDecrypt(NUM_TESTS / fileSizes.size(), filesize, results_encrypt, results_decrypt);
    unsigned long delta = getMilliseconds() - start;
    std::cout << "Tested encrypt, decrypt file size: " << filesize << " bytes, " << NUM_TESTS / fileSizes.size() << " times. Took " << delta << " miliseconds." << std::endl;
  }

  results_encrypt.close();
  std::cout << "Benchmark completed." << std::endl;

  return 0;
}