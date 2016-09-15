#include <string>
#include <iostream>

#include "utils.h"

void challenge_03(){
  std::string cipher = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
  std::string plain = hex2str(cipher);
  std::string key = crackxor(plain);
  std::cout << "key:\t" << key << std::endl;
  std::cout << "plain:\t" << xor_string(plain, key) << std::endl;
}
