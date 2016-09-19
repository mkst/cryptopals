#include <iostream>
#include <vector>
#include <fstream>

#include "stdlib.h"

#include "utils.h"
#include "base64.h"
#include "aes.h"

static const int AES_KEY_LENGTH = 16;

void challenge_20(){

  std::ifstream infile("data/20.txt");
  std::string line;

  std::vector<uint8_t> nonce = { 0, 0, 0, 0, 0, 0, 0, 0 };
  std::vector<uint8_t> key = initialiseiv(16, true);

  std::vector<std::vector<uint8_t>> encrypted;

  size_t minlength = 1000; // none of the lines are longer than this

  while (std::getline(infile, line)) {

    std::vector<uint8_t> base64(line.begin(), line.end());
    std::vector<uint8_t> cipher = b64decode(base64);
    std::vector<uint8_t> e = encrypt_aes_ctr(key, cipher, nonce, 0);

    if(e.size() < minlength) {
      minlength = e.size(); // we will only try to crack 'minlength' worth of cipher
    }
    encrypted.push_back(e);
  }

  // transpose into keylength chunks
  std::vector<std::vector<uint8_t>> transposed(minlength);
  for (size_t i = 0; i < encrypted.size(); i++) {
    for (size_t j = 0; j < minlength; j++) {
      transposed[j].push_back(encrypted[i][j]);
    }
  }

  // use our crackxor function to recover the key
  std::vector<uint8_t> recovered;
  for(size_t i = 0; i < transposed.size(); i++) {
    std::vector<uint8_t> r = crackxor(transposed[i]);
    if(r.size() > 0) {
      recovered.push_back(r[0]);
    }
  }

  // decrypt encrypted ciphers using our recovered key
  for(size_t i = 0; i < encrypted.size(); i++) {
    std::vector<uint8_t> e(encrypted[i].begin(), encrypted[i].begin() + minlength); // only decode the first 'minlength' bytes
    std::cout << v2str(xor_vector(e, recovered)) << std::endl;
  }
}
