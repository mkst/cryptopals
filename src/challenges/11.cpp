#include <string>
#include <random>
#include <iostream>

#include <cassert>

#include "utils.h"
#include "aes.h"

void challenge_11(){
  std::string s1 = "ZaphodBeeblebrox";
  std::string s2 = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

  std::vector<uint8_t> key(s1.begin(), s1.end());
  std::vector<uint8_t> plaintext(s2.begin(), s2.end());

  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> dist(0, 1);

  for(int i = 0; i < 500; i++){ // 500 guesses
    std::vector<uint8_t> e;
    std::vector<uint8_t> plain;
    std::vector<uint8_t> rand1 = padrand(5,10);
    std::vector<uint8_t> rand2 = padrand(5,10);

    // prepend and append random junk
    plain.insert(plain.end(), rand1.begin(), rand1.end());
    plain.insert(plain.end(), plaintext.begin(), plaintext.end());
    plain.insert(plain.end(), rand2.begin(), rand2.end());

    std::string used;
    if((int)dist(mt) % 2) {
      used = "ECB";
      e = encrypt_aes_ecb(key, plain);
    } else {
      used = "CBC";
      e = encrypt_aes_cbc(initialiseiv(16, true), key, plain);
    }
    std::string guess = ecbmatch(e) ? "ECB" : "CBC";
    assert(used == guess);
    }
}
