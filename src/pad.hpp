#ifndef PAD_HPP
#define PAD_HPP

#include <vector>
#include <stdint.h>

using std::vector;

class Pad {
public:
  virtual void pad(const std::vector<uint8_t> &input, std::vector<uint8_t> &output, int NUM_OF_BLOCKBYTE) = 0;
  virtual void unpad(const std::vector<uint8_t> &input, std::vector<uint8_t> &output, int NUM_OF_BLOCKBYTE) = 0;

  virtual ~Pad() {}
};

class PKCS7 : public Pad
{
public:
  void pad(const std::vector<uint8_t> &input, std::vector<uint8_t> &output, int NUM_OF_BLOCKBYTE) override;
  void unpad(const std::vector<uint8_t> &input, std::vector<uint8_t> &output, int NUM_OF_BLOCKBYTE) override;
};

#endif