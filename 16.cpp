#include <string>
#include <iostream>
#include <vector>
#include <regex>

#include "utils.h"
#include "aes.h"

void challenge_16() {

  const int KEY_LENGTH = 16;
  const char PADDING = '-';

  std::string pre = "comment1=cooking%20MCs;userdata=";
  std::string after = ";comment2=%20like%20a%20pound%20of%20bacon";

  std::string input = "XXXXXXXXXXXXXXXX" // this will be mashed
                      "-admin-true-XXXX" // this will be bitflipped
                      "XXXXXX";          // this is just padding

  std::string iv = initialiseiv(KEY_LENGTH, true);
  std::string key = "ZaphodBeeblebrox";

  std::string encrypted = encrypt_aes_cbc(iv, key, pre + regex_replace(input, std::regex(";|="), "") + after);

  std::vector<std::string> v;

  for(int i = 0; i < encrypted.length(); i += KEY_LENGTH) {
    v.push_back(encrypted.substr(i, KEY_LENGTH));
  }

  // flip the bits of "-admin-true-XXXX" to what we want
  v[2][0] = ';' ^ (char)v[2][0] ^ PADDING;
  v[2][6] = '=' ^ (char)v[2][6] ^ PADDING;
  v[2][11] = ';' ^ (char)v[2][11] ^ PADDING;

  std::string bitflipped;
  for(int i = 0; i < v.size(); i++) {
    bitflipped += v[i];
  }

  std::vector<std::string> pairs = explode(decrypt_aes_cbc(iv, key, bitflipped, true), ';');

  for(int i = 0; i < pairs.size(); i++) {
    if (pairs[i] == "admin=true") {
      std::cout << "WE ARE ADMIN!" << std::endl;
    }
  }
}
