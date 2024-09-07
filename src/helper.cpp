#include "helper.hpp"

using std::vector;
using std::string;
using std::array;
using std::reverse;

void string_to_uint8_t_key_array(const std::string &hex_string, std::array<uint8_t, NUM_OF_KEYBYTE> &key_array)
{
  for (int i = 0; i < NUM_OF_KEYBYTE; i+=2)
  {
    try
    {
      unsigned long value = std::stoul(hex_string.substr(i,2), nullptr, 16); // Convert hex string to unsigned long
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

uint32_t rot11(uint32_t x)
{
  return (x << 11) | (x >> (32 - 11));
}