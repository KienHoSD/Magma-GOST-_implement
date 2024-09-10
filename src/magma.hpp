#ifndef __MAGMA_HPP
#define __MAGMA_HPP

#include "magma_ecb.hpp"
#include "magma_cbc.hpp"

class Magma {
public:
  Magma(const std::string &key_hex_string, const std::string &mode = "ECB", const std::string &iv_hex_string = "");

  void change_mode(const std::string &mode, const std::string &iv_hex_string = "");
  void set_sbox(const std::vector<std::vector<uint8_t>> &s_box);

  template <typename T>
  void set_key(const T &key);
  template <typename T>
  void set_iv(const T &iv);
  void encrypt(const std::vector<uint8_t> &plaintext, std::vector<uint8_t> &ciphertext);
  void decrypt(const std::vector<uint8_t> &ciphertext, std::vector<uint8_t> &plaintext);
  void encryptParallel(const std::vector<uint8_t> &plaintext, std::vector<uint8_t> &ciphertext, unsigned int numThreads = 1);
  void decryptParallel(const std::vector<uint8_t> &ciphertext, std::vector<uint8_t> &plaintext, unsigned int numThreads = 1);
private:
  MagmaBase *_magma;
};

#endif
