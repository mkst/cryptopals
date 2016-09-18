#include <string>
#include <iostream>
#include <vector>

#include "utils.h"

void challenge_03(){
  std::string cipher = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";

  std::vector<uint8_t> v1(cipher.begin(), cipher.end());
  std::vector<uint8_t> v2 = hex2v(v1);

  std::vector<uint8_t> key = crackxor(v2);

  std::cout << v2str(xor_vector(v2, key)) << std::endl;
}
