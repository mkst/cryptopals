#include <iostream>

#include "utils.h"

void challenge_09(){
  std::cout << "padded:\t" << str2hex(padme("YELLOW SUBMARINE", 20)) << std::endl;
}
