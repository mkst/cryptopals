#include <string>
#include <iostream>

#include "utils.h"
#include "base64.h"
#include "aes.h"

void challenge_12(){
  std::string key = hex2str("fd0676d8a07bac258e52f886f5158add");

  std::string cipher = b64decode("Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK");

  // 1
  int keylength = encrypt_aes_ecb(key, "a").length();

  // 2
  bool isECB = ecbmatch(encrypt_aes_ecb(key, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));

  // 3

  std::string decrypted;

  for(int z = 0; z < cipher.length(); z += keylength) {

    std::string cipherblock = cipher.substr(z, keylength);
    std::string known;

    for(int j = keylength; j > 0; j--) {
      std::string padding;

      for(int i = 0; i < (j - 1); i++){
        padding += "A";
      }

      for(int i = 1; i < 127; i++) { // why cant we start at 0x00?
        std::string encrypted = encrypt_aes_ecb(key, padding + cipherblock);
        std::string padded    = encrypt_aes_ecb(key, padding + known + std::string(1, (char)i));

        if(encrypted.substr(0,keylength) == padded.substr(0,keylength)) {
          known += (char)i;
          break;
        }
      }
    }
  decrypted += known;
  }
  std::cout << removepadding(decrypted) << std::endl;
  std::cout << cipher << std::endl;
}
