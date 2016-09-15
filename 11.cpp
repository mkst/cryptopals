#include <string>
#include <random>
#include <iostream>

#include "utils.h"
#include "aes.h"

void challenge_11(){
  std::string key = "ZaphodBeeblebrox";
  std::string plaintext = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> dist(0, 1);

  int correct = 0;
  int tries = 256;
  for(int i = 0; i < tries; i++){
    std::string e;
    std::string plain = padrand(5, 10) + plaintext + padrand(5, 10);
    std::string used;
    if((int)dist(mt) % 2) {
      used = "ECB";
      e = encrypt_aes_ecb(key, plain);
    } else {
      used = "CBC";
      e = encrypt_aes_cbc(initialiseiv(16, true), key, plain);
    }
    if(used == (ecbmatch(str2hex(e)) ? "ECB" : "CBC")){
      correct += 1;
    }
  }
  std::cout << "correctly guesssed:\t" << correct << "/" << tries << std::endl;
}
