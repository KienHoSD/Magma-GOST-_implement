#include <iostream>
#include <sys/time.h>
#include <ctime>
#include <vector>
#include "../src/magma.hpp"
#include "../src/pad.hpp"

const unsigned int MICROSECONDS = 1000000;

unsigned long getMicroseconds()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return MICROSECONDS * tv.tv_sec + tv.tv_usec;
}

int main()
{
  const unsigned int MEGABYTE = 1024 * 1024 * sizeof(unsigned char);
  unsigned int megabytesCount = 1;
  unsigned int numThreads = std::thread::hardware_concurrency(); // Use the number of available hardware threads
  unsigned int plainLength = megabytesCount * MEGABYTE * numThreads;
  std::string key = "c54891ee9707aabbd126c79fd47ebb2279ef00473776b219ca3bcd8d06f8cce1";

  std::cout << "Start speedtest multi-thread" << std::endl;
  std::cout << "Number of threads: " << numThreads << std::endl;
  std::cout << "Plain length (MB): " << plainLength / MEGABYTE << std::endl;

  srand(std::time(nullptr));

  std::vector<uint8_t> plain(plainLength);
  std::vector<uint8_t> out(plainLength);

  Magma magmacipher(key);

  unsigned long start = getMicroseconds();
  magmacipher.encryptParallel(plain, out, numThreads);
  unsigned long delta = getMicroseconds() - start;

  double speed = (double)(megabytesCount * numThreads) / delta * MICROSECONDS;
  printf("%.2f Mb/s\n", speed);
  return 0;
}