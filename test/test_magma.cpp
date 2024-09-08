#include "../src/magma.hpp"
#include "../src/pad.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <stdint.h>

using std::vector;
using std::string;
using std::cout;

int main()
{
  Magma magmacipher("c54891ee9707aabbd126c79fd47ebb2279ef00473776b219ca3bcd8d06f8cce1");
  PKCS7 pkcs7;
  string myplain = "the foo in the bar";
  vector<uint8_t> plaintext(myplain.begin(),myplain.end());
  vector<uint8_t> ciphertext(plaintext.size(),255); 

  pkcs7.pad(plaintext,plaintext, NUM_OF_BLOCKBYTE);
  ciphertext.resize(plaintext.size()); // resize to the padded size, give more buffer control to the user

  magmacipher.encrypt(plaintext,ciphertext);
  for(int i=0;i<ciphertext.size();i++) cout<<static_cast<uint32_t>(ciphertext[i])<<" ";
  cout << '\n';

  magmacipher.decrypt(ciphertext,plaintext);
  pkcs7.unpad(plaintext,plaintext, NUM_OF_BLOCKBYTE);
  for(int i=0;i<plaintext.size();i++) cout<<plaintext[i];
  cout << '\n';
}