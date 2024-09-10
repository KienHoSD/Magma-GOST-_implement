#include "magma.hpp"


Magma::Magma(const std::string &key_hex_string, const std::string &mode, const std::string &iv_hex_string)
{
  if (mode == "ECB")
  {
    this->_magma = new MagmaECB(key_hex_string);
  }
  else if (mode == "CBC")
  {
    this->_magma = new MagmaCBC(key_hex_string, iv_hex_string);
  }
  else
  {
    throw std::invalid_argument("Invalid mode");
  }
}

void Magma::set_sbox(const std::vector<std::vector<uint8_t>> &s_box)
{
  this->_magma->set_sbox(s_box);
}

template <typename T>
void Magma::set_key(const T &key)
{
  this->_magma->set_key(key);
}
template void Magma::set_key<std::string>(const std::string &key);
template void Magma::set_key<std::array<uint8_t, NUM_OF_KEYBYTE>>(const std::array<uint8_t, NUM_OF_KEYBYTE> &key);

template <typename T>
void Magma::set_iv(const T &iv)
{
  this->_magma->set_iv(iv);
}
template void Magma::set_iv<std::string>(const std::string &iv);
template void Magma::set_iv<std::array<uint8_t, NUM_OF_IVBYTE>>(const std::array<uint8_t, NUM_OF_IVBYTE> &iv);


void Magma::encrypt(const std::vector<uint8_t> &plaintext, std::vector<uint8_t> &ciphertext)
{
  this->_magma->encrypt(plaintext, ciphertext);
}

void Magma::decrypt(const std::vector<uint8_t> &ciphertext, std::vector<uint8_t> &plaintext)
{
  this->_magma->decrypt(ciphertext, plaintext);
}

void Magma::encryptParallel(const std::vector<uint8_t> &plaintext, std::vector<uint8_t> &ciphertext, unsigned int numThreads)
{
  this->_magma->encryptParallel(plaintext, ciphertext, numThreads);
}

void Magma::decryptParallel(const std::vector<uint8_t> &ciphertext, std::vector<uint8_t> &plaintext, unsigned int numThreads)
{
  this->_magma->decryptParallel(ciphertext, plaintext, numThreads);
}