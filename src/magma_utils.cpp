#include "magma_utils.hpp"

const std::vector<std::vector<uint8_t>> default_s_box = {
  {0xC, 0x4, 0x6, 0x2, 0xA, 0x5, 0xB, 0x9, 0xE, 0x8, 0xD, 0x7, 0x0, 0x3, 0xF, 0x1},
  {0x6, 0x8, 0x2, 0x3, 0x9, 0xA, 0x5, 0xC, 0x1, 0xE, 0x4, 0x7, 0xB, 0xD, 0x0, 0xF},
  {0xB, 0x3, 0x5, 0x8, 0x2, 0xF, 0xA, 0xD, 0xE, 0x1, 0x7, 0x4, 0xC, 0x9, 0x6, 0x0},
  {0xC, 0x8, 0x2, 0x1, 0xD, 0x4, 0xF, 0x6, 0x7, 0x0, 0xA, 0x5, 0x3, 0xE, 0x9, 0xB},
  {0x7, 0xF, 0x5, 0xA, 0x8, 0x1, 0x6, 0xD, 0x0, 0x9, 0x3, 0xE, 0xB, 0x4, 0x2, 0xC},
  {0x5, 0xD, 0xF, 0x6, 0x9, 0x2, 0xC, 0xA, 0xB, 0x7, 0x8, 0x1, 0x4, 0x3, 0xE, 0x0},
  {0x8, 0xE, 0x2, 0x5, 0x6, 0x9, 0x1, 0xC, 0xF, 0x4, 0xB, 0x0, 0xD, 0xA, 0x3, 0x7},
  {0x1, 0x7, 0xE, 0xD, 0x0, 0x5, 0x8, 0x3, 0x4, 0xF, 0xA, 0x6, 0x9, 0xC, 0xB, 0x2}};

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

void string_to_uint8_t_iv_array(const std::string &hex_string, std::array<uint8_t, NUM_OF_IVBYTE> &iv_array)
{
  for (int i = 0; i < NUM_OF_IVBYTE; i++)
  {
    try
    {
      unsigned long value = std::stoul(hex_string.substr(i*2,2), nullptr, 16); // Convert hex string to unsigned long
      if (value > 0xFF)
      {
        throw std::out_of_range("Hex string value is out of range for uint8_t");
      }
      iv_array[i] = static_cast<uint8_t>(value);
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

void block_to_LR(const std::array<uint8_t, NUM_OF_BLOCKBYTE> &block_input, uint32_t &L, uint32_t &R){
  L = (block_input[0] << 24) 
    + (block_input[1] << 16)
    + (block_input[2] << 8)
    + (block_input[3]);
  R = (block_input[4] << 24)
    + (block_input[5] << 16)
    + (block_input[6] << 8)
    + (block_input[7]);
}

void LR_to_block(std::array<uint8_t, NUM_OF_BLOCKBYTE> &block_output, const uint32_t &L, const uint32_t &R){
  for(int i=0;i<NUM_OF_HALFBLOCKBYTE;i++){
    block_output[i] = L >> (NUM_OF_BITSPERHALFBLOCK - NUM_OF_BITSPERBYTE*(i+1)) & 0xff;
    block_output[i+NUM_OF_HALFBLOCKBYTE] = R >> (NUM_OF_BITSPERHALFBLOCK - NUM_OF_BITSPERBYTE*(i+1)) & 0xff;
  }
}

uint32_t rot11(uint32_t x)
{
  return (x << 11) | (x >> (32 - 11));
}