#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <array>

#include <iterator>

#include "cryptopp/aes.h"
using CryptoPP::AES;
#include "cryptopp/modes.h"
using CryptoPP::ECB_Mode;
#include "cryptopp/filters.h"
using CryptoPP::StringSink;

using CryptoPP::ArraySource;
using CryptoPP::StreamTransformationFilter;

#include "utils.h"

static const size_t AES_KEY_LENGTH = 16;

bool validpadding(const std::vector<uint8_t>& v) {
  size_t i = v[v.size() - 1];

  if (0 < i && i <= v.size()) { // sanity
    std::vector<uint8_t> sub(&v[v.size() - i],&v[v.size() - 1]);

    for(size_t j = 0; j < sub.size(); j++){
      if((size_t)sub[j] != i) {
        return false;
      }
    }
    return true;
  }
  return false;
}

std::vector<uint8_t> removepadding(const std::vector<uint8_t>& v) {

  size_t i = v[v.size() - 1];

  if (validpadding(v)) {
    return std::vector<uint8_t>(&v[0],&v[v.size()-i]);
  }
  return v;
}

std::vector<uint8_t> decrypt_aes_ecb(const std::vector<uint8_t>& key, const std::vector<uint8_t>& cipher) {

  std::string recovered;

  ECB_Mode< AES >::Decryption d;

  d.SetKey(key.data(), key.size());

  ArraySource b(cipher.data(), cipher.size(), true,
    new StreamTransformationFilter(d,
      new StringSink(recovered) // FIXME: use ArraySink?
    )
  );

  return std::vector<uint8_t>(recovered.begin(), recovered.end());
}

std::vector<uint8_t> encrypt_aes_ecb(const std::vector<uint8_t>& key, const std::vector<uint8_t>& plaintext) {
  std::string encoded;

  std::vector<uint8_t> padded = padme(plaintext, plaintext.size() + AES_KEY_LENGTH - (plaintext.size() % AES_KEY_LENGTH));

  ECB_Mode< AES >::Encryption e;
  e.SetKey(key.data(), key.size());

  ArraySource(padded.data(), padded.size(), true,
    new StreamTransformationFilter(e,
      new StringSink(encoded), StreamTransformationFilter::NO_PADDING // FIXME: use ArraySink?
    )
  );
  return std::vector<uint8_t>(encoded.begin(), encoded.end());

}

std::vector<uint8_t> encrypt_aes_cbc_block(const std::vector<uint8_t>& iv, const std::vector<uint8_t>& key, const std::vector<uint8_t>& block) {
  const std::vector<uint8_t> x = xor_vector(iv, block);

  std::string encoded;

  ECB_Mode< AES >::Encryption e;
  e.SetKey(key.data(), key.size());

  ArraySource(x.data(), x.size(), true,
    new StreamTransformationFilter(e,
      new StringSink(encoded), StreamTransformationFilter::NO_PADDING // FIXME: use ArraySink
    )
  );

  return std::vector<uint8_t>(encoded.begin(), encoded.end());
}

std::vector<uint8_t> decrypt_aes_cbc_block(const std::vector<uint8_t>& iv, const std::vector<uint8_t>& key, const std::vector<uint8_t>& block){

  std::string decoded;

  ECB_Mode< AES >::Decryption d;
  d.SetKey(key.data(), key.size());

  ArraySource s(block.data(), block.size(), true,
    new StreamTransformationFilter(d,
      new StringSink(decoded), StreamTransformationFilter::NO_PADDING // FIXME: use ArraySink
    )
  );

  return xor_vector(iv, std::vector<uint8_t>(decoded.begin(), decoded.end()));
}

std::vector<uint8_t> encrypt_aes_cbc(const std::vector<uint8_t>& iv, const std::vector<uint8_t>& key, const std::vector<uint8_t>& plaintext) {

  const std::vector<uint8_t> padded = padme(plaintext, plaintext.size() + AES_KEY_LENGTH - (plaintext.size() % AES_KEY_LENGTH));
  std::vector<uint8_t> encrypted;
  std::vector<uint8_t> IV = iv;

  for(size_t i = 0; i < padded.size() ; i += AES_KEY_LENGTH) {
    std::vector<uint8_t> e = encrypt_aes_cbc_block(IV, key, std::vector<uint8_t>(&padded[i], &padded[i+AES_KEY_LENGTH]));
    IV = e;
    encrypted.insert(encrypted.end(), e.begin(), e.end());
   }
  return encrypted;
}

std::vector<uint8_t> decrypt_aes_cbc(const std::vector<uint8_t>& iv, const std::vector<uint8_t>& key, const std::vector<uint8_t>& cipher, const bool removepad) {
  std::vector<uint8_t> decrypted;
  std::vector<uint8_t> IV = iv;
  for(size_t i = 0; i < cipher.size(); i += AES_KEY_LENGTH) {
    std::vector<uint8_t> d = decrypt_aes_cbc_block(IV, key, std::vector<uint8_t>(&cipher[i], &cipher[i+AES_KEY_LENGTH]));
    IV = std::vector<uint8_t>(&cipher[i], &cipher[i+AES_KEY_LENGTH]);
    decrypted.insert(decrypted.end(), d.begin(), d.end());
  }
  return removepad ? removepadding(decrypted) : decrypted;
}

std::vector<uint8_t> crack_aes_cbc(const std::vector<uint8_t>& iv, const std::vector<uint8_t>& key, const std::vector<uint8_t>& cipher1, const std::vector<uint8_t>& cipher2) {

  std::vector<uint8_t> found;

  for(uint8_t pos = 1; pos <= AES_KEY_LENGTH; pos++) { // starting at 1 is easier for indexing

    for(uint16_t i = 0; i < 256; i++){ // use 16bit so we dont overflow

      std::vector<uint8_t> cipher = cipher1; // cipher1 is a const so shove it into cipher

      // fill in the byte we are guessing
      uint8_t c = cipher[AES_KEY_LENGTH - pos];   // start with cipher[15] down to cipher[0]
      uint8_t g = i;                              // the guess
      cipher[AES_KEY_LENGTH - pos] = c ^ pos ^ g; // xor them together with pos (the padding we want!)

      // now fill in the rest of the string
      for(int k = 0; k < pos - 1; k++) {
        uint8_t c = cipher[AES_KEY_LENGTH - k - 1];   // e.g. cipher[14]
        uint8_t g = found[k];                         // previously good value
        cipher[AES_KEY_LENGTH - k - 1] = c ^ pos ^ g; // xor them together with pos (the padding we want!)
      }

      // create 2-block cipher to be decrypted
      cipher.insert(cipher.end(), cipher2.begin(), cipher2.end());

      // do the work and see if we got a valid padding
      std::vector<uint8_t> decrypted = decrypt_aes_cbc(iv, key, cipher, false);
      if(decrypted[2*AES_KEY_LENGTH -1] == pos && validpadding(decrypted)) {
        found.push_back(i);
      }
    }
  }

  std::reverse(found.begin(), found.end()); // flip
  return found;
}

std::vector<uint8_t> keystream(const std::vector<uint8_t>& nonce, const int counter) {

  std::vector<uint8_t> n = nonce;

  int i = counter;

  if(counter > 4294967295) {
    throw ("We are going to overflow!");
  }

  while(i>0) {
    div_t d = div(i,16);
    i = d.quot;
    n.push_back(d.rem);
  }
  return padzero(n, AES_KEY_LENGTH);
}

std::vector<uint8_t> decrypt_aes_ctr(const std::vector<uint8_t>& key, const std::vector<uint8_t>& cipher, const std::vector<uint8_t>& nonce, const int counter) {

  std::vector<uint8_t> decrypted;

  int ctr = counter;

  for (size_t i = 0; i < cipher.size(); i += AES_KEY_LENGTH) {

    size_t chunksize = AES_KEY_LENGTH;

    if(i+AES_KEY_LENGTH > cipher.size()){
      // we are at the final chunk
      chunksize = cipher.size() % AES_KEY_LENGTH;
    }
    std::vector<uint8_t> e = encrypt_aes_ecb(key, keystream(nonce, ctr));
    std::vector<uint8_t> c = std::vector<uint8_t>(cipher.begin() + i, cipher.begin() + i + chunksize);

    std::vector<uint8_t> x = xor_vector(c, std::vector<uint8_t>(e.begin(), e.begin() + AES_KEY_LENGTH)); // ignore 16 bytes of padding

    decrypted.insert(decrypted.end(), x.begin(), x.begin() + chunksize);

    // increment stream counter
    ctr++;
  }

  return decrypted;
}
std::vector<uint8_t> encrypt_aes_ctr(const std::vector<uint8_t>& key, const std::vector<uint8_t>& plaintext, const std::vector<uint8_t>& nonce, const int counter) {

  std::vector<uint8_t> encrypted;
  int ctr = counter;

  div_t d = div(plaintext.size(),16);

  for (int i = 0; i < d.quot; i++) {

    std::vector<uint8_t> e = encrypt_aes_ecb(key, keystream(nonce, ctr));
    encrypted.insert(encrypted.end(), e.begin(), e.begin() + AES_KEY_LENGTH);

    // increment stream counter
    ctr++;
  }

  // add any remaing chunk
  if(d.rem > 0) {
    std::vector<uint8_t> e = encrypt_aes_ecb(key, keystream(nonce, ctr));
    encrypted.insert(encrypted.end(), e.begin(), e.begin() + d.rem);
  }

  return xor_vector(encrypted, plaintext);
}
