#ifndef __MAGMA_UTILS_HPP
#define __MAGMA_UTILS_HPP

#include <vector>
#include <string>
#include <stdint.h>
#include <iostream>
#include <array>
#include <algorithm>
#include <thread>
#include <future>
#include <cstring>
#include "constants.h"

extern const std::vector<std::vector<uint8_t>> default_s_box;

void string_to_uint8_t_key_array(const std::string &hex_string, std::array<uint8_t, NUM_OF_KEYBYTE> &key_array);

void string_to_uint8_t_iv_array(const std::string &hex_string, std::array<uint8_t, NUM_OF_IVBYTE> &iv_array);

void block_to_LR(const std::array<uint8_t, NUM_OF_BLOCKBYTE> &block_input, uint32_t &L, uint32_t &R);

void LR_to_block(std::array<uint8_t, NUM_OF_BLOCKBYTE> &block_output, const uint32_t &L, const uint32_t &R);

uint32_t rot11(uint32_t x);

#endif