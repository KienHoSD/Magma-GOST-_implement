#include "pad.hpp"
#include <iostream>

void PKCS7::pad(const std::vector<uint8_t> &input, std::vector<uint8_t> &output, int NUM_OF_BLOCKBYTE)
{
  output = input;
  int pad_length = NUM_OF_BLOCKBYTE - input.size() % NUM_OF_BLOCKBYTE;
  for (int i = 0; i < pad_length; i++)
  {
    output.push_back(pad_length);
  }
}

void PKCS7::unpad(const std::vector<uint8_t> &input, std::vector<uint8_t> &output, int NUM_OF_BLOCKBYTE)
{
  output = input;
  int pad_length = output.back();
  if(pad_length > NUM_OF_BLOCKBYTE) {
    std::cerr << "Invalid padding" << std::endl;
    return;
  }
  output.resize(output.size() - pad_length);
}