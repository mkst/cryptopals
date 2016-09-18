#include <string>
#include <iostream>
#include <vector>

#include "utils.h"

void challenge_02(){
  const std::string s1 = "1c0111001f010100061a024b53535009181c";
  const std::string s2 = "686974207468652062756c6c277320657965";

  // convert strings to vectors
  const std::vector<uint8_t> v1(s1.begin(), s1.end());
  const std::vector<uint8_t> v2(s2.begin(), s2.end());

  // xor the vectors
  const std::vector<uint8_t> v3 = xor_vector(hex2v(v1), hex2v(v2));

  // print result to console
  std::cout << v2hex(v3) << std::endl;
}
