#ifndef __MAGMA_ECB_HPP
#define __MAGMA_ECB_HPP

#include "magma_base.hpp"

class MagmaECB : public MagmaBase {
public:
  using MagmaBase::MagmaBase;

  void encrypt(const std::vector<uint8_t> &plaintext, std::vector<uint8_t> &ciphertext) override;
  void decrypt(const std::vector<uint8_t> &ciphertext, std::vector<uint8_t> &plaintext) override;
  void encryptParallel(const std::vector<uint8_t> &plaintext, std::vector<uint8_t> &ciphertext, unsigned int numThreads = 1) override;
  void decryptParallel(const std::vector<uint8_t> &ciphertext, std::vector<uint8_t> &plaintext, unsigned int numThreads = 1) override;

private:
  void inner_encrypt_decrypt(const std::vector<uint8_t> &input, std::vector<uint8_t> &output, const bool &is_encrypt);
  void inner_encrypt_decrypt_parallel(const std::vector<uint8_t> &input, std::vector<uint8_t> &output, const unsigned int &numThreads, const bool &is_encrypt);
};

#endif