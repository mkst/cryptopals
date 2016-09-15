#include <string>
#include <iostream>

#include "utils.h"
#include "base64.h"

void challenge_01(){
  const std::string hex = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
  std::string plain = hex2str(hex);
  std::string b64 = b64encode(plain);
  std::cout << "hex:\t" << hex << std::endl;
  std::cout << "plain:\t" << plain << std::endl;
  std::cout << "b64:\t" << b64 << std::endl;
}
