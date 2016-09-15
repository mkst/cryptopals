#include <string>
#include <iostream>
#include <fstream>

#include "aes.h"
#include "base64.h"

void challenge_07(){
  std::ifstream f("data/07.txt");
  std::string str((std::istreambuf_iterator<char>(f)),std::istreambuf_iterator<char>());
  std::string cipher = b64decode(str);
  std::string key = "YELLOW SUBMARINE";
  std::string decrypted = decrypt_aes_ecb(key, cipher);
  std::cout << "decrypted:\t" << decrypted << std::endl;
}
