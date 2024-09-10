#ifndef __MAGMA_CBC_HPP
#define __MAGMA_CBC_HPP

#include "magma_base.hpp"

class MagmaCBC : public MagmaBase {
public:
  MagmaCBC(const std::string &key_hex_string, const std::array<uint8_t, NUM_OF_IVBYTE> &iv_array);
  MagmaCBC(const std::string &key_hex_string, const std::string &iv_hex_string);

  void encrypt(const std::vector<uint8_t> &plaintext, std::vector<uint8_t> &ciphertext) override;
  void decrypt(const std::vector<uint8_t> &ciphertext, std::vector<uint8_t> &plaintext) override;
  void set_iv(const std::string &iv_hex_string) override;
  void set_iv(const std::array<uint8_t, NUM_OF_IVBYTE> &iv_array) override;

private:
  std::array<uint8_t, NUM_OF_IVBYTE> iv_array;

  void encrypt_block(const std::vector<uint8_t> &input, std::vector<uint8_t> &output, std::array<uint8_t, NUM_OF_BLOCKBYTE> &prev_block, size_t current_block_index);
  void decrypt_block(const std::vector<uint8_t> &input, std::vector<uint8_t> &output, std::array<uint8_t, NUM_OF_BLOCKBYTE> &prev_block, size_t current_block_index);
  void inner_encrypt_decrypt(const std::vector<uint8_t> &input, std::vector<uint8_t> &output, const bool &is_encrypt);
};

#endif