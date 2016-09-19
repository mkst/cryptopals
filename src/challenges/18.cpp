#include <string>
#include <iostream>
#include <vector>

#include "utils.h"
#include "base64.h"
#include "aes.h"

static const int AES_KEY_LENGTH = 16;

void challenge_18(){

  std::vector<uint8_t> nonce = { 0, 0, 0, 0, 0, 0, 0, 0 };
  int counter = 0;

  /*
  std::string s1 = "ZaphodBeeblebrox";
  std::string s2 = "Far out in the uncharted backwat"
                   "ers of the unfashionable end of "
                   "the western spiral arm of the Ga"
                   "laxy lies a small, unregarded ye"
                   "llow sun.";

  std::vector<uint8_t> key(s1.begin(), s1.end());
  std::vector<uint8_t> plaintext(s2.begin(), s2.end());

  std::cout << v2str(decrypt_aes_ctr(key, encrypt_aes_ctr(key, plaintext, nonce, counter), nonce, counter)) << std::endl;
  */

  std::string s1 = "L77na/nrFsKvynd6HzOoG7GHTLXsTVu9qvY/2syLXzhPweyyMTJULu/6/kXX0KSvoOLSFQ==";
  std::string s2 = "YELLOW SUBMARINE";

  std::vector<uint8_t> base64 = std::vector<uint8_t>(s1.begin(), s1.end());
  std::vector<uint8_t> cipher = b64decode(base64);
  std::vector<uint8_t> key(s2.begin(), s2.end());

  std::cout << v2str(decrypt_aes_ctr(key, cipher, nonce, counter)) << std::endl;
}
