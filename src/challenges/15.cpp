#include <string>
#include <vector>
#include <iostream>

#include "aes.h"

void challenge_15() {

  std::string s1 = "ICE ICE BABY\x04\x04\x04\x04";
  std::string s2 = "\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10";
  std::string s3 = "ICE ICE BABY\x05\x05\x05\x05";
  std::string s4 = "ICE ICE BABY\x01\x02\x03\x04";

  std::cout << validpadding(std::vector<uint8_t>(s1.begin(), s1.end())) << std::endl;
  std::cout << validpadding(std::vector<uint8_t>(s2.begin(), s2.end())) << std::endl;
  std::cout << validpadding(std::vector<uint8_t>(s3.begin(), s3.end())) << std::endl;
  std::cout << validpadding(std::vector<uint8_t>(s4.begin(), s4.end())) << std::endl;
}
