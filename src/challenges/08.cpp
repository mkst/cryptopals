#include <string>
#include <fstream>
#include <iostream>

#include "utils.h"

void challenge_08(){
  std::ifstream infile("data/08.txt");
  std::string line;

  while (std::getline(infile, line)) {
    std::vector<uint8_t> v1(line.begin(), line.end());
    std::vector<uint8_t> v2 = hex2v(v1);

    if(ecbmatch(v2)) {
      std::cout << line << std::endl; // lazy
      break;
    }
  }
}
