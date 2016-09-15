#pragma once

std::string decrypt_aes_ecb(const std::string& key, const std::string& cipher);
std::string encrypt_aes_ecb(const std::string& key, const std::string& plaintext);

std::string encrypt_aes_cbc_block(const std::string& iv, const std::string& key, const std::string& block);
std::string decrypt_aes_cbc_block(const std::string& iv, const std::string& key, const std::string& block);

std::string decrypt_aes_cbc(const std::string& iv, const std::string& key, const std::string& cipher, const bool removepad);
std::string encrypt_aes_cbc(const std::string& iv, const std::string& key, const std::string& plaintext);

std::string crack_aes_cbc(const std::string& iv, const std::string& key, const std::string& cipher1, const std::string& cipher2);

bool validpadding(const std::string& s);
std::string removepadding(const std::string& s);
