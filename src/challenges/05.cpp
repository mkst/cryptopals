#include <string>
#include <iostream>

#include "utils.h"

void challenge_05(){
  std::string plain = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
  std::cout << "encrypted:\t" << str2hex(xor_string(plain, "ICE")) << std::endl;
}

