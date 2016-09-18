#include <string>
#include <iostream>
#include <vector>

#include "utils.h"

void challenge_09(){

  std::string s = "YELLOW SUBMARINE";
  int length = 20;

  std::vector<uint8_t> key(s.begin(), s.end());

  std::cout << v2hex(padme(key, length)) << std::endl;
}
