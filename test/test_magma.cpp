#include "../src/magma.hpp"
#include "../src/pad.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <stdint.h>

using std::cerr;
using std::cout;
using std::string;
using std::vector;

void test_mode(const string &mode, const string &key, const string &iv, const string &plaintext_str)
{
  try
  {
    Magma magmacipher(key, mode, iv);
    PKCS7 pkcs7;
    vector<uint8_t> plaintext(plaintext_str.begin(), plaintext_str.end());
    vector<uint8_t> ciphertext;

    pkcs7.pad(plaintext, plaintext, NUM_OF_BLOCKBYTE);
    magmacipher.encrypt(plaintext, ciphertext);

    cout << "Ciphertext (" << mode << "): ";
    for (size_t i = 0; i < ciphertext.size(); ++i)
    {
      cout << static_cast<uint32_t>(ciphertext[i]) << " ";
    }
    cout << '\n';

    magmacipher.decrypt(ciphertext, plaintext);
    pkcs7.unpad(plaintext, plaintext, NUM_OF_BLOCKBYTE);

    cout << "Decrypted text (" << mode << "): ";
    for (size_t i = 0; i < plaintext.size(); ++i)
    {
      cout << plaintext[i];
    }
    cout << '\n';
  }
  catch (const std::exception &e)
  {
    cerr << "Exception in mode " << mode << ": " << e.what() << '\n';
  }
  catch (...)
  {
    cerr << "Unknown exception in mode " << mode << '\n';
  }
}

int main()
{
  string key = "c54891ee9707aabbd126c79fd47ebb2279ef00473776b219ca3bcd8d06f8cce1";
  string iv = "aabbccddeeff0011";
  string plaintext_str = "the foo in the bar";

  cout << "Key: " << key << '\n';
  cout << "IV: " << iv << '\n';
  cout << "Plaintext: " << plaintext_str << '\n';

  test_mode("ECB", key, "", plaintext_str);
  test_mode("CBC", key, iv, plaintext_str);
  // test_mode("CFB", key, iv, plaintext_str);
  // test_mode("OFB", key, iv, plaintext_str);

  return 0;
}