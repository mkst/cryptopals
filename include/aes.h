#pragma once

#include <vector>

std::vector<uint8_t> decrypt_aes_ecb(const std::vector<uint8_t>& key, const std::vector<uint8_t>& cipher);
std::vector<uint8_t> encrypt_aes_ecb(const std::vector<uint8_t>& key, const std::vector<uint8_t>& plaintext);

std::vector<uint8_t> decrypt_aes_cbc(const std::vector<uint8_t>& iv, const std::vector<uint8_t>& key, const std::vector<uint8_t>& cipher, const bool removepad);
std::vector<uint8_t> encrypt_aes_cbc(const std::vector<uint8_t>& iv, const std::vector<uint8_t>& key, const std::vector<uint8_t>& plaintext);

std::vector<uint8_t> decrypt_aes_ctr(const std::vector<uint8_t>& key, const std::vector<uint8_t>& cipher, const std::vector<uint8_t>& nonce, const int counter);
std::vector<uint8_t> encrypt_aes_ctr(const std::vector<uint8_t>& key, const std::vector<uint8_t>& plaintext, const std::vector<uint8_t>& nonce, const int counter);


std::vector<uint8_t> crack_aes_cbc(const std::vector<uint8_t>& iv, const std::vector<uint8_t>& key, const std::vector<uint8_t>& cipher1, const std::vector<uint8_t>& cipher2);

bool validpadding(const std::vector<uint8_t>& v);
std::vector<uint8_t> removepadding(const std::vector<uint8_t>& v);
std::vector<uint8_t> keystream(const std::vector<uint8_t>& nonce, const int counter);
