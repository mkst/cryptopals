#include <string>
#include <fstream>
#include <iostream>

#include "utils.h"

void challenge_08(){
  std::ifstream infile("data/08.txt");
  std::string line;
  std::string res;

  while (std::getline(infile, line)) {
    if(ecbmatch(line)) {
      res = line;
      break;
    }
  }
  if(!res.empty()){
    // TODO: crack this one?
    std::cout << "found:\t" << res << std::endl;
  }
}
