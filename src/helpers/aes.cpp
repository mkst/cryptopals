#include <string>
#include <iostream>

#include "cryptopp/aes.h"
using CryptoPP::AES;
#include "cryptopp/modes.h"
using CryptoPP::ECB_Mode;
#include "cryptopp/filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;

#include "utils.h"

bool validpadding(const std::string& s) {
  size_t i = (int)s[s.length() - 1];

  if (0 < i && i <= s.length()) { // sanity
    std::string sub = s.substr(s.length() - i, i);

    for(size_t j = 0; j < sub.length(); j++){
      if((size_t)sub[j] != i) {
        return false;
      }
    }
    return true;
  }
  return false;
}

std::string removepadding(const std::string& s) {

  size_t i = (int)s[s.length() - 1];

  if (validpadding(s)) {
    return s.substr(0, (s.length() - i));
  }
  return s;
}

std::string decrypt_aes_ecb(const std::string& key, const std::string& cipher){
  std::string recovered;

  ECB_Mode< AES >::Decryption d;
  d.SetKey(reinterpret_cast<const byte*>(key.data()), key.length());

  StringSource s(cipher, true,
    new StreamTransformationFilter(d,
      new StringSink(recovered)
    )
  );

  return recovered;
}

std::string encrypt_aes_ecb(const std::string& key, const std::string& plaintext){
  std::string encoded;

  std::string padded = padme(plaintext, plaintext.length() + 16 - (plaintext.length() % 16));

  ECB_Mode< AES >::Encryption e;
  e.SetKey(reinterpret_cast<const byte*>(key.data()), key.length());

  StringSource(padded.data(), true,
    new StreamTransformationFilter(e,
      new StringSink(encoded), StreamTransformationFilter::NO_PADDING
    )
  );
  return encoded;
}

std::string encrypt_aes_cbc_block(const std::string& iv, const std::string& key, const std::string& block) {
  const std::string x = xor_string(iv, block);

  std::string encoded;

  ECB_Mode< AES >::Encryption e;
  e.SetKey(reinterpret_cast<const byte*>(key.data()), key.length());

  StringSource(x, true,
    new StreamTransformationFilter(e,
      new StringSink(encoded), StreamTransformationFilter::NO_PADDING
    )
  );

  //std::cout << "iv:  " << str2hex(iv) << " blk: " << str2hex(block) << " x:   " << str2hex(x) << " enc: " << str2hex(encoded) << std::endl;
  return encoded;
}

std::string decrypt_aes_cbc_block(const std::string& iv, const std::string& key, const std::string& block){

  std::string decoded;

  ECB_Mode< AES >::Decryption d;
  d.SetKey(reinterpret_cast<const byte*>(key.data()), key.length());

  StringSource s(block, true,
    new StreamTransformationFilter(d,
      new StringSink(decoded), StreamTransformationFilter::NO_PADDING
    )
  );

  std::string x = xor_string(iv, decoded);

  return x;
}

std::string encrypt_aes_cbc(const std::string& iv, const std::string& key, const std::string& plaintext) {

  const std::string padded = padme(plaintext, plaintext.length() + 16 - (plaintext.length() % 16));
  std::string encrypted;
  std::string IV = iv;

  for(size_t i = 0; i < padded.length() ; i += 16) {
    std::string e = encrypt_aes_cbc_block(IV, key, padded.substr(i, 16));
    IV = e;
    encrypted += e;
   }
  return encrypted;
}

std::string decrypt_aes_cbc(const std::string& iv, const std::string& key, const std::string& cipher, const bool removepad) {
  std::string decrypted;
  std::string IV = iv;
  for(size_t i = 0; i < cipher.length(); i += 16) {
    std::string d = decrypt_aes_cbc_block(IV, key, cipher.substr(i,16));
    IV = cipher.substr(i,16);
    decrypted += d;
  }
  return removepad ? removepadding(decrypted) : decrypted;
}

std::string crack_aes_cbc(const std::string& iv, const std::string& key, const std::string& cipher1, const std::string& cipher2) {

  std::string found;

  const int KEY_LENGTH = 16;

  for(int pos = 1; pos <= KEY_LENGTH; pos++) { // starting at 1 is easier for indexing

    for(int i = 0; i < 256; i++){

      std::string cipher = cipher1; // cipher1 is a const so shove it into cipher

      // fill in the byte we are guessing
      char c = cipher[KEY_LENGTH - pos];      // start with cipher[15] down to cipher[0]
      char g = i;                             // the guess
      cipher[KEY_LENGTH - pos] = c ^ pos ^ g; // xor them together with pos (the padding we want!)

      // now fill in the rest of the string
      for(int k = 0; k < pos - 1; k++) {
        char c = cipher[KEY_LENGTH - k - 1];      // e.g. cipher[14]
        char g = found[k];                        // previously good value
        cipher[KEY_LENGTH - k - 1] = c ^ pos ^ g; // xor them together with pos (the padding we want!)
      }

      // do the work and see if we got a valid padding
      std::string decrypted = decrypt_aes_cbc(iv, key, cipher + cipher2, false);
      if(decrypted[2*KEY_LENGTH -1] == pos && validpadding(decrypted)) {
        found += (char)i;
      }
    }
  }

  std::reverse(found.begin(), found.end()); // flip
  return found;
}
