#include "magma_base.hpp"

using std::array;
using std::cerr;
using std::cin;
using std::cout;
using std::invalid_argument;
using std::out_of_range;
using std::reverse;
using std::stoul;
using std::string;
using std::vector;

MagmaBase::MagmaBase(const std::string &key_hex_string)
{
  set_key(key_hex_string);
}

MagmaBase::MagmaBase(const std::array<uint8_t, NUM_OF_KEYBYTE> &key_array)
{
  this->key_array = key_array;
  key_schedule();
}

uint32_t MagmaBase::f(const uint32_t &Ri, const uint32_t &Ki)
{
  uint32_t res1 = 0, res2 = 0, res3 = 0;
  res1 = Ri + Ki; // already module 2^32 due to data type uint32_t
  for (int i = 0; i < NUM_OF_HALFBLOCKNIBBLE; i++)
  {
    res2 += this->s_box[i][res1 >> (NUM_OF_BITSPERHALFBLOCK - NUM_OF_BITSPERNIBBLE * (i + 1)) & 0xf] << NUM_OF_BITSPERNIBBLE * (NUM_OF_HALFBLOCKNIBBLE - i - 1);
  }
  res3 = rot11(res2);
  return res3;
}

void MagmaBase::round_function(uint32_t &L0, uint32_t &R0, const bool &is_encrypt)
{
  uint32_t Li, Ri, Lip1 = L0, Rip1 = R0;
  int round_to_swap = is_encrypt ? 24 : 8;
  for (int round = 0; round < NUM_OF_ROUNDS; round++)
  {
    Li = Lip1;
    Ri = Rip1;
    if (round == round_to_swap)
    {
      reverse(key_block.begin(), key_block.end());
    }
    if (is_encrypt)
    {
      Lip1 = Ri;
      Rip1 = Li ^ f(Ri, key_block[round % NUM_OF_KEYBLOCK]);
    }
    else
    {
      Lip1 = Ri ^ f(Li, key_block[round % NUM_OF_KEYBLOCK]);
      Rip1 = Li;
    }
  }
  reverse(key_block.begin(), key_block.end());
  L0 = Lip1;
  R0 = Rip1;
}

void MagmaBase::set_sbox(const vector<vector<uint8_t>> &s_box){
  if(s_box.size() != NUM_OF_HALFBLOCKNIBBLE){
    throw invalid_argument("Invalid s_box size");
    return;
  }
  for(int i=0;i<NUM_OF_HALFBLOCKNIBBLE;i++){
    if(s_box[i].size() != 16){
      throw invalid_argument("Invalid s_box[i] size");
      return;
    }
  }
  this->s_box = s_box;
}

void MagmaBase::set_key(const std::string &key_hex_string)
{
  if (key_hex_string.length() != NUM_OF_KEYBYTE * 2)
    throw std::runtime_error("Key (hex) length must be " + std::to_string(NUM_OF_KEYBYTE * 2));
    
  string_to_uint8_t_key_array(key_hex_string, key_array);
  key_schedule();
}

void MagmaBase::set_key(const std::array<uint8_t, NUM_OF_KEYBYTE> &key_array)
{
  if(key_array.size() != NUM_OF_KEYBYTE)
    throw std::runtime_error("Key (array) length must be " + std::to_string(NUM_OF_KEYBYTE));

  this->key_array = key_array;
  key_schedule();
}

void MagmaBase::key_schedule()
{
  for (int i = 0; i < NUM_OF_KEYBLOCK; i++)
  {
    key_block[i] = 0;
    for (int j = 0; j < NUM_OF_KEYBLOCKBYTE; j++)
    {
      key_block[i] <<= NUM_OF_BITSPERBYTE;
      key_block[i] += key_array[i * NUM_OF_KEYBLOCKBYTE + j];
    }
  }
}
