#include <string>
#include <iostream>

#include <cassert>

#include "utils.h"
#include "base64.h"
#include "aes.h"

void challenge_12(){

  std::string s1 = "fd0676d8a07bac258e52f886f5158add";
  std::string s2 = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK";

  std::vector<uint8_t> key = hex2v(std::vector<uint8_t>(s1.begin(), s1.end()));

  std::vector<uint8_t> cipher = b64decode(std::vector<uint8_t>(s2.begin(), s2.end()));

  // 1
  size_t keylength = encrypt_aes_ecb(key, std::vector<uint8_t>({'a'})).size();

  // 2
  std::string aaa = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
  assert(ecbmatch(encrypt_aes_ecb(key, std::vector<uint8_t>(aaa.begin(), aaa.end()))));

  // 3
  std::vector<uint8_t> decrypted;

  for(size_t z = 0; z < cipher.size(); z += keylength) {

    std::vector<uint8_t> cipherblock = std::vector<uint8_t>(&cipher[z], &cipher[z+keylength]);
    std::vector<uint8_t> known;

    for(size_t j = keylength; j > 0; j--) {
      std::vector<uint8_t> padding;

      for(size_t i = 0; i < (j - 1); i++){
        padding.push_back('A');
      }

      std::vector<uint8_t> p = padding;
      p.insert(p.end(), cipherblock.begin(), cipherblock.end());
      std::vector<uint8_t> encrypted = encrypt_aes_ecb(key, p);

      // we only care about the first block
      encrypted = std::vector<uint8_t>(encrypted.begin(), encrypted.begin() + keylength);

      for(int i = 0; i < 256; i++) {
        // guess plaintext
        p = padding;
        p.insert(p.end(), known.begin(), known.end());
        p.push_back(i);

        std::vector<uint8_t> padded(encrypt_aes_ecb(key, p));

        padded = std::vector<uint8_t>(padded.begin(), padded.begin() + keylength);

        if(encrypted  == padded) {
          known.push_back(i);
          break;
        }
      }
    }
  decrypted.insert(decrypted.end(), known.begin(), known.end());
  }
  std::cout << v2str(removepadding(decrypted)) << std::endl;
}
