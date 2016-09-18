#include <string>
#include <vector>
#include <regex>
#include <iostream>

#include "utils.h"
#include "aes.h"

static const size_t AES_KEY_LENGTH = 16;

std::string profile_for(const std::string& s){
  std::vector<std::string> v;

  std::regex bad("&|=");
  std::string sanitized = regex_replace(s, bad, "");

  // too lazy to genuinely create object
  v.push_back("email=" + sanitized);
  v.push_back("uid=10");
  v.push_back("role=user");

  return encodev(v, '&');
}

void challenge_13(){

  std::string s1 = "YELLOW SUBMARINE";
  std::vector<uint8_t> key(s1.begin(), s1.end());

  // craft the attack
  std::string a = "me@hacker.";
  std::string s = "admin";
  std::string b = v2str(padme(std::vector<uint8_t>(s.begin(), s.end()), 16));
  std::string c = "com";

  std::string profile = profile_for(a + b + c);

  std::vector<uint8_t> encrypted = encrypt_aes_ecb(key, std::vector<uint8_t>(profile.begin(), profile.end()));

  // encrypted[0] = email=xxxxxxxxxx
  // encrypted[1] = adminBBBBBBBBBBB
  // encrypted[2] = xxx&uid=10&role=
  // encrypted[3] = userXXXXXXXXXXXX

  std::vector<uint8_t> crafted;

  crafted.insert(crafted.end(), encrypted.begin(), encrypted.begin() + AES_KEY_LENGTH);
  crafted.insert(crafted.end(), encrypted.begin() + 2*AES_KEY_LENGTH, encrypted.begin() + 3*AES_KEY_LENGTH);
  crafted.insert(crafted.end(), encrypted.begin() + AES_KEY_LENGTH, encrypted.begin() + 2*AES_KEY_LENGTH);

  std::cout << encodev(parsekv((v2str(decrypt_aes_ecb(key, crafted))), '&'), '&') << std::endl;
}
