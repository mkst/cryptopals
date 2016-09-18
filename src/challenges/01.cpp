#include <string>
#include <iostream>
#include <vector>

#include "utils.h"
#include "base64.h"

void challenge_01(){
  const std::string hex = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";

  std::vector<uint8_t> v1(hex.begin(), hex.end()); // create vector from string
  std::vector<uint8_t> v2 = hex2v(v1);             // convert from hex
  std::vector<uint8_t> v3 = b64encode(v2);         // base64 encode

  std::cout << v2str(v3) << std::endl;             // print to console
}
