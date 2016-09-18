#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "utils.h"

void challenge_04(){
  std::ifstream infile("data/04.txt");

  std::vector<uint8_t> key;
  std::vector<uint8_t> cipher;

  std::string line;

  while (std::getline(infile, line)) {

    std::vector<uint8_t> v(line.begin(), line.end());

    cipher = hex2v(v);
    key = crackxor(cipher);
    if(!key.empty()) {
      break;
    }
  }

  std::cout << v2str(xor_vector(cipher, key)) << std::endl;
}
