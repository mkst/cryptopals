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

  std::vector<uint8_t> iv = initialiseiv(KEY_LENGTH, true);
  std::string s1 = "ZaphodBeeblebrox";

  std::vector<uint8_t> key(s1.begin(), s1.end());

  // seed the random generator
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> dist(0, plaintexts.size());

  // pick a random plaintext to encrypt
  int random = dist(mt);
  std::string plaintext = plaintexts[random];

  // then encrypt it
  std::vector<uint8_t> encrypted = encrypt_aes_cbc(iv, key, std::vector<uint8_t>(plaintext.begin(), plaintext.end()));

  // use IV for first block, we could guess this
  std::vector<uint8_t> cracked = crack_aes_cbc(iv, key, iv, std::vector<uint8_t>(encrypted.begin(), encrypted.begin() + KEY_LENGTH));

  // work through the blocks and crack each pair
  for(size_t i = 0; i < encrypted.size() - KEY_LENGTH; i += KEY_LENGTH) {
    std::vector<uint8_t> c = crack_aes_cbc(iv, key, std::vector<uint8_t>(encrypted.begin() + i, encrypted.begin() + i + KEY_LENGTH),
                                      std::vector<uint8_t>(encrypted.begin() + i + KEY_LENGTH, encrypted.begin() + i + 2*KEY_LENGTH));
    cracked.insert(cracked.end(), c.begin(), c.end());
  }

  std::cout << v2str(removepadding(cracked)) << std::endl;
}
