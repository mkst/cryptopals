#include <string>
#include <iostream>
#include <vector>
#include <regex>

#include "utils.h"
#include "aes.h"

static const size_t AES_KEY_LENGTH = 16;

void challenge_16() {

  const char PADDING = '-';

  std::string s1 = "ZaphodBeeblebrox"; // key

  std::string pre = "comment1=cooking%20MCs;userdata=";
  std::string post = ";comment2=%20like%20a%20pound%20of%20bacon";

  std::string input = "XXXXXXXXXXXXXXXX" // this will be corrupted
                      "-admin-true-XXXX" // this will be bitflipped
                      "XXXXXX";          // this is just padding

  std::string attack = pre + regex_replace(input, std::regex(";|="), "") + post;

  std::vector<uint8_t> iv = initialiseiv(AES_KEY_LENGTH, false);
  std::vector<uint8_t> key(s1.begin(), s1.end());

  std::vector<uint8_t> encrypted = encrypt_aes_cbc(iv, key, std::vector<uint8_t>(attack.begin(), attack.end()));

  // XXXXXXXXXXXXXXXX
  // -admin-true-XXXX
  // ^     ^    ^
  // do the bit flip on bytes 0, 6 and 11
  encrypted[2*AES_KEY_LENGTH] = (';' ^ encrypted[2*AES_KEY_LENGTH] ^ PADDING);
  encrypted[2*AES_KEY_LENGTH + 6] = ('=' ^ encrypted[2*AES_KEY_LENGTH + 6] ^ PADDING);
  encrypted[2*AES_KEY_LENGTH + 11] = (';' ^ encrypted[2*AES_KEY_LENGTH + 11] ^ PADDING);

  // now to decrypt and check the result
  std::vector<std::string> pairs = explode(v2str(decrypt_aes_cbc(iv, key, encrypted, true)), ';');

  for(size_t i = 0; i < pairs.size(); i++) {
    if (pairs[i] == "admin=true") {
      std::cout << "WE ARE ADMIN!" << std::endl;
    }
  }
}
