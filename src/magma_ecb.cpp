#include "magma_ecb.hpp"

void MagmaECB::inner_encrypt_decrypt(const std::vector<uint8_t> &input, std::vector<uint8_t> &output, const bool &is_encrypt){
  if (input.size() % NUM_OF_BLOCKBYTE != 0){
    throw std::runtime_error("Input length must be a multiple of " + std::to_string(NUM_OF_BLOCKBYTE));
  }

  // matching output to input size
  output.resize(input.size());
  
  uint32_t L,R;
  std::array<uint8_t, NUM_OF_BLOCKBYTE> block_temp;
  for(size_t current_block_index = 0; current_block_index < input.size(); current_block_index += NUM_OF_BLOCKBYTE){
    std::copy(input.begin() + current_block_index,
              input.begin() + current_block_index + NUM_OF_BLOCKBYTE,
              block_temp.begin());
    block_to_LR(block_temp, L, R);
    round_function(L, R, is_encrypt);
    LR_to_block(block_temp, L, R);
    std::copy(block_temp.begin(),
              block_temp.end(),
              output.begin() + current_block_index);
  }
}

void MagmaECB::encrypt(const std::vector<uint8_t> &plaintext, std::vector<uint8_t> &ciphertext) {
  inner_encrypt_decrypt(plaintext,ciphertext,true);
}

void MagmaECB::decrypt(const std::vector<uint8_t> &ciphertext, std::vector<uint8_t> &plaintext) {
  inner_encrypt_decrypt(ciphertext,plaintext,false);
}

void MagmaECB::inner_encrypt_decrypt_parallel(const std::vector<uint8_t> &input, std::vector<uint8_t> &output, const unsigned int &numThreads, const bool &is_encrypt){
  if (input.size() % NUM_OF_BLOCKBYTE != 0){
    throw std::runtime_error("Input length must be a multiple of " + std::to_string(NUM_OF_BLOCKBYTE));
  }

  output.resize(input.size());
  unsigned int chunkSize = (input.size() / numThreads) / NUM_OF_BLOCKBYTE * NUM_OF_BLOCKBYTE;
  std::vector<std::future<void>> futures;

  for (unsigned int i = 0; i < numThreads; ++i)
  {
    unsigned int start = i * chunkSize;
    unsigned int end = (i == numThreads - 1) ? input.size() : start + chunkSize;

    futures.push_back(std::async(std::launch::async, [this, &input, &output, start, end, is_encrypt]()  // Capture is_encrypt in the lambda's capture list
    {
      MagmaECB threadLocalMagma(this->key_array);  // Each thread creates its own instance of Magma
      std::vector<uint8_t> chunkOutput(end - start);
      threadLocalMagma.inner_encrypt_decrypt(
        std::vector<uint8_t>(input.begin() + start, input.begin() + end),
        chunkOutput, 
        is_encrypt  // Encrypt mode
      );
      std::copy(chunkOutput.begin(), chunkOutput.end(), output.begin() + start); 
    }));
  }

  for (auto &future : futures)
  {
    future.get();
  }
}

void MagmaECB::encryptParallel(const std::vector<uint8_t> &plaintext, std::vector<uint8_t> &ciphertext, unsigned int numThreads){
  inner_encrypt_decrypt_parallel(plaintext,ciphertext,numThreads,true);
}

void MagmaECB::decryptParallel(const std::vector<uint8_t> &ciphertext, std::vector<uint8_t> &plaintext, unsigned int numThreads){
  inner_encrypt_decrypt_parallel(ciphertext,plaintext,numThreads,false);
}