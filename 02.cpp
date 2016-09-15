#include <string>
#include <iostream>

#include "utils.h"

void challenge_02(){
  const std::string p = "1c0111001f010100061a024b53535009181c";
  const std::string c = "686974207468652062756c6c277320657965";
  std::string x = xor_string(hex2str(p), hex2str(c));
  std::cout << "cipher:\t" << hex2str(c) << std::endl;
  std::cout << "plain:\t" << x << std::endl;
  std::cout << "hex:\t" << str2hex(x) << std::endl;
}
