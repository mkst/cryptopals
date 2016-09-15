#include <string>
#include <iostream>
#include <fstream>

#include "utils.h"
#include "base64.h"

void challenge_06(){

  //std::string a = "this is a test";
  //std::string b = "wokka wokka!!!";
  //std::cout << "ham distance:\t" << getham(a, b) << std::endl;

  std::ifstream f("data/06.txt");
  std::string str((std::istreambuf_iterator<char>(f)),std::istreambuf_iterator<char>());
  std::string plain = b64decode(str);
  int keylength = guesskeylength(plain, 40);

  // initialise vector
  std::vector<std::string> v;
  for(int i = 0; i < keylength; i++){
    v.push_back("");
  }

  // transpose into keylength chunks
  for(int i = 0; i < plain.length(); i++) {
    v[i % keylength] += plain[i];
  }

  // crack each chunk
  std::string key = "";
  for(int i = 0; i < keylength; i++) {
    std::string res = crackxor(v[i]);
    if ( res != "" ){
      key += res;
    }
   }

  if(key.length() != keylength) {
    throw("Failed to decode...");
  }

  std::cout << "keylength:\t " << keylength << std::endl;
  std::cout << "key:\t" << key << std::endl;
  std::cout << "cracked:\t" << xor_string(plain, key) << std::endl;
}
