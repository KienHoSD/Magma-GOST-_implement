#ifndef HELPER_HPP
#define HELPER_HPP

#include <vector>
#include <string>
#include <stdint.h>
#include <iostream>
#include <array>
#include <algorithm>

const int NUM_OF_BITSPERBYTE = 8;
const int NUM_OF_BITSPERNIBBLE = 4;
const int NUM_OF_BLOCKBYTE = 8;
const int NUM_OF_KEYBYTE = 32;
const int NUM_OF_KEYBLOCK = 8;
const int NUM_OF_ROUNDS = 32;
const int NUM_OF_HALFBLOCKBYTE = 4;
const int NUM_OF_HALFBLOCKNIBBLE = 8;
const int NUM_OF_KEYBLOCKBYTE = 4;
const int NUM_OF_BITSPERBLOCK = 64;
const int NUM_OF_BITSPERHALFBLOCK = 32;

void string_to_uint8_t_key_array(const std::string &hex_string, std::array<uint8_t, NUM_OF_KEYBYTE> &key_array);

uint32_t rot11(uint32_t x);

#endif