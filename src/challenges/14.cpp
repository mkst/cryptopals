#include <string>
#include <iostream>
#include <vector>

#include "utils.h"
#include "base64.h"
#include "aes.h"

static std::vector<uint8_t> RANDOM_PREFIX;

std::vector<uint8_t> encrypt_aes_ecb_prefix(const std::vector<uint8_t>& key, const std::vector<uint8_t>& cipher) {

  std::vector<uint8_t> r = RANDOM_PREFIX;
  r.insert(r.end(), cipher.begin(), cipher.end());
  return encrypt_aes_ecb(key, r);
}

void challenge_14(){

  // This is similar to 12, but we are doing:
  // AES-128-ECB(random-prefix || attacker-controlled || target-bytes, random-key)
  // so the plan is | RANDOM_PREFIX ... PADDING | AAAAAAAAAAAAAAA* | TARGET |

  // initialise random prefix
  RANDOM_PREFIX = padrand(0,40);

  std::string s1 = "fd0676d8a07bac258e52f886f5158add";
  std::string s2 = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK";

  std::vector<uint8_t> key = hex2v(std::vector<uint8_t>(s1.begin(), s1.end()));
  std::vector<uint8_t> cipher = b64decode(std::vector<uint8_t>(s2.begin(), s2.end()));

  size_t keylength = 16; // we know this from before

  std::vector<uint8_t> encrypted;

  // pad ciphertext to 16-byte blocksize and encrypt
  std::vector<uint8_t> c = cipher;
  for(size_t i = 0; i < keylength - (cipher.size() % keylength); i++){
    c.push_back('#');
  }

  // this is the basis for finding the length of the random prefix
  encrypted = encrypt_aes_ecb_prefix(key, c);
  size_t padded_size = encrypted.size();

  // work out how many keylengths worth of padding we require
  div_t d = div(padded_size - cipher.size(), keylength);
  size_t PADDING_BLOCKS = d.quot;

  // now we want to pad 'c' until we generate another keylength block
  size_t original_csize = c.size();
  size_t target_size = padded_size + keylength;

  while(encrypted.size() < target_size){
    c.push_back('#');
    encrypted = encrypt_aes_ecb_prefix(key, c);
  }

  // and now we have figured it out!
  size_t padding_required = c.size() - original_csize;

  // pad a prefix so (random+prefix % keylength) == 0
  std::vector<uint8_t> prefix;
  for(size_t i = 0; i < padding_required; i++){
    prefix.push_back('#');
  }

  // the cracking is similar to 12, just need to take the correct encrypted block
  std::vector<uint8_t> decrypted;
  for(size_t z = 0; z < cipher.size(); z += keylength) {

    std::vector<uint8_t> cipherblock = std::vector<uint8_t>(&cipher[z], &cipher[z+keylength]);
    std::vector<uint8_t> known;

    for(size_t j = keylength; j > 0; j--) {
      std::vector<uint8_t> padding = prefix;

      for(size_t i = 0; i < (j - 1); i++){
        padding.push_back('A');
      }

      std::vector<uint8_t> p = padding;
      p.insert(p.end(), cipherblock.begin(), cipherblock.end());
      encrypted = encrypt_aes_ecb_prefix(key, p);

      // ignore the blocks we know are padded
      encrypted = std::vector<uint8_t>(encrypted.begin() + PADDING_BLOCKS*keylength, encrypted.begin() + (1+PADDING_BLOCKS)*keylength);

      for(int i = 0; i < 256; i++) {
        // guess plaintext
        p = padding;
        p.insert(p.end(), known.begin(), known.end());
        p.push_back(i);

        std::vector<uint8_t> padded(encrypt_aes_ecb_prefix(key, p));

        // ignore the bocks we know are padded
        padded = std::vector<uint8_t>(padded.begin() + PADDING_BLOCKS*keylength, padded.begin() + (1+PADDING_BLOCKS)*keylength);

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
