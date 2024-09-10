#ifndef __MAGMA_BASE_HPP
#define __MAGMA_BASE_HPP

#include "magma_utils.hpp"

class MagmaBase
{
public:
  MagmaBase(const std::string &key_hex_string);
  MagmaBase(const std::array<uint8_t, NUM_OF_KEYBYTE> &key_array);
  virtual ~MagmaBase() = default;

  virtual void encrypt(const std::vector<uint8_t> &plaintext, std::vector<uint8_t> &ciphertext) = 0;
  virtual void decrypt(const std::vector<uint8_t> &ciphertext, std::vector<uint8_t> &plaintext) = 0;
  virtual void encryptParallel(const std::vector<uint8_t> &plaintext, std::vector<uint8_t> &ciphertext, unsigned int numThreads = 1) {
    throw std::runtime_error("Parallel encryption is not supported for this mode");
  }
  virtual void decryptParallel(const std::vector<uint8_t> &ciphertext, std::vector<uint8_t> &plaintext, unsigned int numThreads = 1) {
    throw std::runtime_error("Parallel decryption is not supported for this mode");
  }

  void set_key(const std::string &key_hex_string);
  void set_key(const std::array<uint8_t, NUM_OF_KEYBYTE> &key_array);
  virtual void set_iv(const std::string &iv_hex_string) {
    throw std::runtime_error("IV is not supported for this mode");
  }
  virtual void set_iv(const std::array<uint8_t, NUM_OF_IVBYTE> &iv_array) {
    throw std::runtime_error("IV is not supported for this mode");
  }
  void set_sbox(const std::vector<std::vector<uint8_t>> &s_box);

protected:
  std::array<uint8_t, NUM_OF_KEYBYTE> key_array;
  std::array<uint32_t, NUM_OF_KEYBLOCK> key_block;
  std::vector<std::vector<uint8_t>> s_box = default_s_box;

  void key_schedule();
  uint32_t f(const uint32_t &Ri, const uint32_t &Ki);
  void round_function(uint32_t &L0, uint32_t &R0, const bool &is_encrypt);
};

#endif
