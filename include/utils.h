#pragma once

#include <vector>
#include <string>

std::string hex2str(const std::string& h);
std::string str2hex(const std::string& s);

std::vector<uint8_t> xor_vector(const std::vector<uint8_t>& v1, const std::vector<uint8_t>& v2);

std::vector<uint8_t> crackxor(const std::vector<uint8_t>& v);
int guesskeylength(const std::vector<uint8_t>& s, int maxlength);
std::vector<uint8_t> padme(const std::vector<uint8_t>& v, int length);
std::vector<uint8_t> initialiseiv(uint8_t length, bool randomise);

bool ecbmatch(std::vector<uint8_t> v);
std::vector<uint8_t> padrand(int min, int max);

const std::vector<std::string> explode(const std::string& s, const char& c);
std::vector<std::string> parsekv(const std::string& s, const char split);
std::string encodev(const std::vector<std::string> v, const char join);

std::string v2hex(const std::vector<uint8_t>& v);
std::vector<uint8_t> hex2v(const std::vector<uint8_t>& h);
std::string v2str(const std::vector<uint8_t>& v);
