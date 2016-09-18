#include <string>
#include <iostream>
#include <vector>

#include "utils.h"

void challenge_05(){
  std::string s1 = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
  std::string s2 = "ICE";

  std::vector<uint8_t> plain(s1.begin(), s1.end());
  std::vector<uint8_t> key(s2.begin(), s2.end());

  std::cout << v2hex(xor_vector(plain,key)) << std::endl;
}

