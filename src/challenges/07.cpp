#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "aes.h"
#include "base64.h"
#include "utils.h"

void challenge_07(){
  std::ifstream f("data/07.txt");
  std::string s1((std::istreambuf_iterator<char>(f)),std::istreambuf_iterator<char>());
  std::string s2 = "YELLOW SUBMARINE";

  std::vector<uint8_t> v1(s1.begin(), s1.end());

  std::vector<uint8_t> cipher = b64decode(v1);
  std::vector<uint8_t> key(s2.begin(), s2.end());

  std::vector<uint8_t> v2 = decrypt_aes_ecb(key, cipher);

  std::cout << v2str(v2) << std::endl;
}
