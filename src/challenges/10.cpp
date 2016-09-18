#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "aes.h"
#include "base64.h"
#include "utils.h"

void challenge_10(){

  /*
  std::vector<uint8_t> iv = initialiseiv(16, true);

  std::string s1 = "ZaphodBeeblebrox";
  std::string s2 = "Far out in the uncharted backwat"
                   "ers of the unfashionable end of "
                   "the western spiral arm of the Ga"
                   "laxy lies a small, unregarded ye"
                   "llow sun.";

  std::vector<uint8_t> key(s1.begin(), s1.end());
  std::vector<uint8_t> plaintext(s2.begin(), s2.end());

  std::cout << v2str(decrypt_aes_ecb(key, encrypt_aes_ecb(key, plaintext))) << std::endl;
  std::cout << v2str(decrypt_aes_cbc(iv, key, encrypt_aes_cbc(iv, key, plaintext), true)) << std::endl;
  */

  std::ifstream f("data/10.txt");
  std::string s1((std::istreambuf_iterator<char>(f)),std::istreambuf_iterator<char>());
  std::string s2 = "YELLOW SUBMARINE";

  std::vector<uint8_t> plain = b64decode(std::vector<uint8_t>(s1.begin(), s1.end()));
  std::vector<uint8_t> key(s2.begin(), s2.end());

  std::cout << v2str(decrypt_aes_cbc(initialiseiv(16, false), key, plain, true)) << std::endl;
}
