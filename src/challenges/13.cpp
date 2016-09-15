#include <string>
#include <vector>
#include <regex>
#include <iostream>

#include "utils.h"
#include "aes.h"

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

  std::string a = "me@hacker.";
  std::string b = padme("admin", 16);
  std::string c = "com";

  std::string fudge = encrypt_aes_ecb("YELLOW SUBMARINE", profile_for(a + b + c));
  std::vector<std::string> v;

  v.push_back(fudge.substr(0,16));  // email=xxxxxxxxxx
  v.push_back(fudge.substr(16,16)); // adminBBBBBBBBBBB
  v.push_back(fudge.substr(32,16)); // xxx&uid=10&role=
  v.push_back(fudge.substr(48,16)); // userXXXXXXXXXXXX

  std::cout << "original: " << str2hex(fudge) << std::endl;
  std::cout << "tweaked:  " << str2hex(v[0] + v[2] + v[1]) << std::endl;
  std::cout << "result:   " << encodev(parsekv((decrypt_aes_ecb("YELLOW SUBMARINE", v[0] + v[2] + v[1])), '&'), '&') << std::endl;
}

