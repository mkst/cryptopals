#include <string>
#include <iostream>

#include "aes.h"

void challenge_15() {
  std::cout << "valid:\t" << validpadding("ICE ICE BABY\x04\x04\x04\x04") << std::endl;
  std::cout << "valid:\t" << validpadding("\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10") << std::endl;
  std::cout << "not:\t" << validpadding("ICE ICE BABY\x05\x05\x05\x05") << std::endl;
  std::cout << "not:\t" << validpadding("ICE ICE BABY\x01\x02\x03\x04") << std::endl;
}
