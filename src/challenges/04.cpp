#include <string>
#include <iostream>
#include <fstream>

#include "utils.h"

void challenge_04(){
  std::ifstream infile("data/04.txt");
  std::string line;
  std::string key;
  std::string cipher;

  while (std::getline(infile, line)) {
    cipher = hex2str(line);
    key = crackxor(cipher);
    if(!key.empty()) {
      break;
    }
  }

  std::string plain = xor_string(cipher, key);

  std::cout << "key:\t" << str2hex(key) << std::endl;
  std::cout << "hex:\t" << str2hex(plain) << std::endl;
  std::cout << "plain:\t" << plain << std::endl;
}
