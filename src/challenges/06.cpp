#include <string>
#include <iostream>
#include <fstream>

#include <cassert>

#include "utils.h"
#include "base64.h"

void challenge_06(){

  std::ifstream f("data/06.txt");
  std::string s1((std::istreambuf_iterator<char>(f)),std::istreambuf_iterator<char>());

  std::vector<uint8_t> v1(s1.begin(), s1.end()); // TODO: stream straight into the vector
  std::vector<uint8_t> v2 = b64decode(v1);

  // guess the key length
  size_t keylength = guesskeylength(v2, 40);

  // transpose into keylength chunks
  std::vector<std::vector<uint8_t>> v3(keylength);
  for (size_t i = 0; i < v2.size(); i++) {
    v3[i % keylength].push_back(v2[i]);
  }

  // crack each chunk
  std::vector<uint8_t> key;
  for (size_t i = 0; i < keylength; i++) {
    std::vector<uint8_t> res = crackxor(v3[i]);
    if (res.size() > 0) {
      key.push_back(res[0]); // add cracked key to full key
    }
   }

  assert(key.size() == keylength);

  std::cout << v2str(xor_vector(v2, key)) << std::endl;
}
