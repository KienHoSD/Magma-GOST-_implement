#include "helper.hpp"

using std::vector;
using std::string;
using std::array;
using std::reverse;

void string_to_uint8_t_key_array(const std::string &hex_string, std::array<uint8_t, NUM_OF_KEYBYTE> &key_array)
{
  for (int i = 0; i < NUM_OF_KEYBYTE; i++)
  {
    try
    {
      unsigned long value = std::stoul(hex_string.substr(i*2,2), nullptr, 16); // Convert hex string to unsigned long
      if (value > 0xFF)
      {
        throw std::out_of_range("Hex string value is out of range for uint8_t");
      }
      key_array[i] = static_cast<uint8_t>(value);
    }
    catch (const std::invalid_argument &e)
    {
      throw std::runtime_error("Invalid hex string");
    }
    catch (const std::out_of_range &e)
    {
      throw std::runtime_error("Hex string is out of range");
    }
  }
}

void block_to_LR(const std::vector<uint8_t> &input, const size_t &block_index, uint32_t &L, uint32_t &R){
  L = (input[block_index]   << 24) 
    + (input[block_index+1] << 16)
    + (input[block_index+2] << 8)
    + (input[block_index+3]);
  R = (input[block_index+4] << 24)
    + (input[block_index+5] << 16)
    + (input[block_index+6] << 8)
    + (input[block_index+7]);
}


void LR_to_block(std::vector<uint8_t> &output, const size_t &block_index, const uint32_t &L, const uint32_t &R){
  for(int i=0;i<NUM_OF_HALFBLOCKBYTE;i++){
    output[block_index+i] = L >> (NUM_OF_BITSPERHALFBLOCK - NUM_OF_BITSPERBYTE*(i+1)) & 0xff;
    output[block_index+i+NUM_OF_HALFBLOCKBYTE] = R >> (NUM_OF_BITSPERHALFBLOCK - NUM_OF_BITSPERBYTE*(i+1)) & 0xff;
  }
}

uint32_t rot11(uint32_t x)
{
  return (x << 11) | (x >> (32 - 11));
}