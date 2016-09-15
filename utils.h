#pragma once

#include <vector>
#include <string>

std::string hex2str(const std::string& h);
std::string str2hex(const std::string& s);

std::string xor_string(const std::string& plaintext, const std::string& key);
bool allprintable(const std::string& s);
int score(const std::string& s);
std::string crackxor(const std::string& s);
int countbits(const std::string& x);
int getham(const std::string& a, const std::string& b);
int guesskeylength(const std::string& s, int maxlength);
std::string padme(const std::string& s, int length);
std::string initialiseiv(int size, bool randomise);
bool ecbmatch(std::string hex);
std::string padrand(int min, int max);
const std::vector<std::string> explode(const std::string& s, const char& c);
std::vector<std::string> parsekv(const std::string& s, const char split);
std::string encodev(const std::vector<std::string> v, const char join);
