#include <string>
#include <iostream>
#include <vector>
#include <random>

#include "base64.h"
#include "utils.h"
#include "aes.h"

void challenge_17(){

  const int KEY_LENGTH = 16;
  std::vector<std::string> plaintexts;

  plaintexts.push_back(b64decode("MDAwMDAwTm93IHRoYXQgdGhlIHBhcnR5IGlzIGp1bXBpbmc="));
  plaintexts.push_back(b64decode("MDAwMDAxV2l0aCB0aGUgYmFzcyBraWNrZWQgaW4gYW5kIHRoZSBWZWdhJ3MgYXJlIHB1bXBpbic="));
  plaintexts.push_back(b64decode("MDAwMDAyUXVpY2sgdG8gdGhlIHBvaW50LCB0byB0aGUgcG9pbnQsIG5vIGZha2luZw=="));
  plaintexts.push_back(b64decode("MDAwMDAzQ29va2luZyBNQydzIGxpa2UgYSBwb3VuZCBvZiBiYWNvbg=="));
  plaintexts.push_back(b64decode("MDAwMDA0QnVybmluZyAnZW0sIGlmIHlvdSBhaW4ndCBxdWljayBhbmQgbmltYmxl"));
  plaintexts.push_back(b64decode("MDAwMDA1SSBnbyBjcmF6eSB3aGVuIEkgaGVhciBhIGN5bWJhbA=="));
  plaintexts.push_back(b64decode("MDAwMDA2QW5kIGEgaGlnaCBoYXQgd2l0aCBhIHNvdXBlZCB1cCB0ZW1wbw=="));
  plaintexts.push_back(b64decode("MDAwMDA3SSdtIG9uIGEgcm9sbCwgaXQncyB0aW1lIHRvIGdvIHNvbG8="));
  plaintexts.push_back(b64decode("MDAwMDA4b2xsaW4nIGluIG15IGZpdmUgcG9pbnQgb2g="));
  plaintexts.push_back(b64decode("MDAwMDA5aXRoIG15IHJhZy10b3AgZG93biBzbyBteSBoYWlyIGNhbiBibG93"));

  std::string iv = initialiseiv(KEY_LENGTH, true);
  std::string key = "ZaphodBeeblebrox";

  // seed the random generator
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> dist(0, plaintexts.size());

  // pick a random plaintext to encrypt
  int random = dist(mt);
  std::string encrypted = encrypt_aes_cbc(iv, key, plaintexts[random]);

  // vector to put ciphertext in
  std::vector<std::string> v;

  for(size_t i = 0; i < encrypted.length(); i += KEY_LENGTH) {
    v.push_back(encrypted.substr(i, KEY_LENGTH));
  }

  std::string cracked = crack_aes_cbc(iv, key, iv, v[0]); // use IV for first block, we could guess this
  for(size_t i = 0; i < v.size() - 1; i++) {
    cracked += crack_aes_cbc(iv, key, v[i], v[i+1]);
  }

  std::cout << "cracked:\t" << str2hex(cracked) << std::endl;
  std::cout << "unhexed:\t" << removepadding(cracked) << std::endl;
  std::cout << "decrypt:\t" << str2hex(decrypt_aes_cbc(iv, key, encrypted, false)) << std::endl;
  std::cout << "original:\t" << plaintexts[random] << std::endl;
}
