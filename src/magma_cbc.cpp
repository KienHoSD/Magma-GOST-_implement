#include "magma_cbc.hpp"

MagmaCBC::MagmaCBC(const std::string &key_hex_string, const std::array<uint8_t, NUM_OF_IVBYTE> &iv_array)
  : MagmaBase(key_hex_string) {
  set_iv(iv_array);
}

MagmaCBC::MagmaCBC(const std::string &key_hex_string, const std::string &iv_hex_string)
  : MagmaBase(key_hex_string) {
  set_iv(iv_hex_string);
}

void MagmaCBC::inner_encrypt_decrypt(const std::vector<uint8_t> &input, std::vector<uint8_t> &output, const bool &is_encrypt){
  if (input.size() % NUM_OF_BLOCKBYTE != 0)
    throw std::runtime_error("Input length must be a multiple of " + std::to_string(NUM_OF_BLOCKBYTE));

  // matching output to input size
  output.resize(input.size());
  
  uint32_t L,R;
  std::array<uint8_t, NUM_OF_BLOCKBYTE> block_temp, prev_block=iv_array, xor_block;
  for(size_t current_block_index = 0; current_block_index < input.size(); current_block_index += NUM_OF_BLOCKBYTE){
    std::copy(input.begin() + current_block_index,
              input.begin() + current_block_index + NUM_OF_BLOCKBYTE,
              block_temp.begin());

    xor_block = prev_block;
    if(is_encrypt){
      for(size_t i=0;i<NUM_OF_BLOCKBYTE;i++){
        block_temp[i] ^= xor_block[i];
      }
    }
    else{
      prev_block = block_temp;
    }

    block_to_LR(block_temp, L, R);
    round_function(L, R, is_encrypt);
    LR_to_block(block_temp, L, R);

    if(is_encrypt){
      prev_block = block_temp;
    }
    else{
      for(size_t i=0;i<NUM_OF_BLOCKBYTE;i++){
        block_temp[i] ^= xor_block[i];
      }
    }
    
    std::copy(block_temp.begin(),
              block_temp.end(),
              output.begin() + current_block_index);
  }
}

void MagmaCBC::encrypt(const std::vector<uint8_t> &plaintext, std::vector<uint8_t> &ciphertext) {
  inner_encrypt_decrypt(plaintext,ciphertext,true);
}

void MagmaCBC::decrypt(const std::vector<uint8_t> &ciphertext, std::vector<uint8_t> &plaintext) {
  inner_encrypt_decrypt(ciphertext,plaintext,false);
}

void MagmaCBC::set_iv(const std::string &iv_hex_string){
  if(iv_hex_string.empty())
    throw std::runtime_error("IV (hex) must not be empty");
  if (iv_hex_string.size() != NUM_OF_IVBYTE*2)
    throw std::runtime_error("IV (hex) length must be " + std::to_string(NUM_OF_IVBYTE*2));
  string_to_uint8_t_iv_array(iv_hex_string, this->iv_array);
}

void MagmaCBC::set_iv(const std::array<uint8_t, NUM_OF_IVBYTE> &iv_array){
  if(iv_array.size() != NUM_OF_IVBYTE)
    throw std::runtime_error("IV (array) length must be " + std::to_string(NUM_OF_IVBYTE));
  this->iv_array = iv_array;
}