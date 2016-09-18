#pragma once

#include <string>
#include <vector>

std::string b64encode(const std::string& s);
std::string b64decode(const std::string& d);
std::vector<uint8_t> b64encode(const std::vector<uint8_t>& v);
std::vector<uint8_t> b64decode(const std::vector<uint8_t>& v);
