#include "magma.hpp"
#include "helper.hpp"

using std::array;
using std::cerr;
using std::cin;
using std::cout;
using std::string;
using std::invalid_argument;
using std::out_of_range;
using std::stoul;
using std::vector;
using std::reverse;

const vector<vector<uint8_t>> s_box = {
  {0xC, 0x4, 0x6, 0x2, 0xA, 0x5, 0xB, 0x9, 0xE, 0x8, 0xD, 0x7, 0x0, 0x3, 0xF, 0x1},
  {0x6, 0x8, 0x2, 0x3, 0x9, 0xA, 0x5, 0xC, 0x1, 0xE, 0x4, 0x7, 0xB, 0xD, 0x0, 0xF},
  {0xB, 0x3, 0x5, 0x8, 0x2, 0xF, 0xA, 0xD, 0xE, 0x1, 0x7, 0x4, 0xC, 0x9, 0x6, 0x0},
  {0xC, 0x8, 0x2, 0x1, 0xD, 0x4, 0xF, 0x6, 0x7, 0x0, 0xA, 0x5, 0x3, 0xE, 0x9, 0xB},
  {0x7, 0xF, 0x5, 0xA, 0x8, 0x1, 0x6, 0xD, 0x0, 0x9, 0x3, 0xE, 0xB, 0x4, 0x2, 0xC},
  {0x5, 0xD, 0xF, 0x6, 0x9, 0x2, 0xC, 0xA, 0xB, 0x7, 0x8, 0x1, 0x4, 0x3, 0xE, 0x0},
  {0x8, 0xE, 0x2, 0x5, 0x6, 0x9, 0x1, 0xC, 0xF, 0x4, 0xB, 0x0, 0xD, 0xA, 0x3, 0x7},
  {0x1, 0x7, 0xE, 0xD, 0x0, 0x5, 0x8, 0x3, 0x4, 0xF, 0xA, 0x6, 0x9, 0xC, 0xB, 0x2}
};


uint32_t Magma::f(const uint32_t &Ri, const uint32_t &Ki){
  uint32_t res1=0, res2=0, res3=0;
  res1 = Ri + Ki; // already module 2^32 due to data type uint32_t
  for(int i=0;i<NUM_OF_HALFBLOCKNIBBLE;i++){
    res2 += s_box[i][res1 >> (NUM_OF_BITSPERHALFBLOCK - NUM_OF_BITSPERNIBBLE*(i+1)) & 0xf] << NUM_OF_BITSPERNIBBLE*(NUM_OF_HALFBLOCKNIBBLE-i-1);
  }
  res3 = rot11(res2);
  return res3;
}

void Magma::inner_encrypt_decrypt(const vector<uint8_t> &input, vector<uint8_t> &output, bool is_encrypt){
  if (input.size() % NUM_OF_BLOCKBYTE != 0){
    throw std::runtime_error("Input length must be a multiple of 8");
    return;
  }

  for(int block_number = 0;block_number*NUM_OF_BLOCKBYTE < input.size();block_number++){
    int current_index = block_number*NUM_OF_BLOCKBYTE;
    uint32_t L0 = (input[current_index]   << 24) 
                + (input[current_index+1] << 16)
                + (input[current_index+2] << 8)
                + (input[current_index+3]);
    uint32_t R0 = (input[current_index+4] << 24)
                + (input[current_index+5] << 16)
                + (input[current_index+6] << 8)
                + (input[current_index+7]);
    uint32_t Li;
    uint32_t Ri;
    uint32_t Lip1=L0;
    uint32_t Rip1=R0;

    int round_to_swap = is_encrypt ? 24 : 8;
    for(int round = 0; round < NUM_OF_ROUNDS; round++){
      Li = Lip1;
      Ri = Rip1;
      if(round == round_to_swap){
        reverse(key_block.begin(),key_block.end());
      }
      if(is_encrypt){
        Lip1 = Ri;
        Rip1 = Li ^ f(Ri,key_block[round%NUM_OF_KEYBLOCK]);
      }
      else{
        Lip1 = Ri ^ f(Li,key_block[round%NUM_OF_KEYBLOCK]);
        Rip1 = Li;
      }
    }
    reverse(key_block.begin(),key_block.end());

    for(int i=0;i<NUM_OF_HALFBLOCKBYTE;i++){
      output[current_index+i] = Lip1 >> (NUM_OF_BITSPERHALFBLOCK - NUM_OF_BITSPERBYTE*(i+1)) & 0xff;
      output[current_index+i+NUM_OF_HALFBLOCKBYTE] = Rip1 >> (NUM_OF_BITSPERHALFBLOCK - NUM_OF_BITSPERBYTE*(i+1)) & 0xff;
    }
  }
}

void Magma::encrypt(const vector<uint8_t> &plaintext, vector<uint8_t> &ciphertext){
  inner_encrypt_decrypt(plaintext,ciphertext,true);
}

void Magma::decrypt(const vector<uint8_t> &ciphertext, vector<uint8_t> &plaintext){
  inner_encrypt_decrypt(ciphertext,plaintext,false);
}

void Magma::set_sbox(const vector<vector<uint8_t>> &s_box){
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

void Magma::key_schedule()
{
  for(int i=0;i<NUM_OF_KEYBLOCK;i++){
    for(int j=0;j<NUM_OF_KEYBLOCKBYTE;j++){
      key_block[i] <<= NUM_OF_BITSPERBYTE;
      key_block[i] += key_array[i*NUM_OF_KEYBLOCK+j];
    }
  }
}
