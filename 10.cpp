#include <string>
#include <iostream>
#include <fstream>

#include "aes.h"
#include "base64.h"
#include "utils.h"

void challenge_10(){

  std::string iv = initialiseiv(16, true);
  std::string key = "ZaphodBeeblebrox";

  std::string plaintext = "Far out in the uncharted backwat"
                          "ers of the unfashionable end of "
                          "the western spiral arm of the Ga"
                          "laxy lies a small, unregarded ye"
                          "llow sun.";

  std::cout << "ori : " << str2hex(plaintext) << std::endl;
  std::cout << "cbc : " << str2hex(decrypt_aes_cbc(iv, key, (encrypt_aes_cbc(iv, key, plaintext)), true)) << std::endl;
  std::cout << "ecb : " << str2hex(decrypt_aes_ecb(key, (encrypt_aes_ecb(key, plaintext)))) << std::endl;

  std::ifstream f("data/10.txt");
  std::string str((std::istreambuf_iterator<char>(f)),std::istreambuf_iterator<char>());
  std::string plain = b64decode(str);
  std::cout << "decrypted:\t" << decrypt_aes_cbc(initialiseiv(16, false), "YELLOW SUBMARINE", plain, true) << std::endl;

}
