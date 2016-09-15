//#include <string>   // for string
#include <sstream>  // for stringstream
#include <iomanip>  // for setw and setfill
#include <iostream> // for cout
#include <map>
#include <random>

std::string hex2str(const std::string& h) {
  std::string s;

  if(h.length() & 1) {
    throw("Tried to string a non hex string");
  }

  for(int i = 0; i < h.length(); i += 2) {
    int j = 0;
    std::stringstream ss(h.substr(i,2));
    ss >> std::hex >> j;
    s += char(j);
  }
  return s;
}

std::string xor_string(const std::string& plaintext, const std::string& key) {
  std::string x;
  for(int i = 0; i < plaintext.length(); i++){
    x += (char) (plaintext[i] ^ key[i % key.length()]);
  }
  return x;
}

std::string str2hex(const std::string& s) {
  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  for (int i = 0; s.length() > i; ++i) {
      ss << std::setw(2) << static_cast<unsigned int>(static_cast<unsigned char>(s[i]));
  }
  return ss.str();
}

bool allprintable(const std::string& s) {
  for(int i = 0; i < s.length(); i++) {
    if(s[i] > 127) {
      return false; // anything non-ASCII
    }
  }
  return true;
}

int score(const std::string& s) {
  int j = 0;
  for(int i = 0; i < s.length(); i++) {
    int x = s[i];
    if ((x > 64 && x < 91) ||  // A-Z
        (x > 96 && x < 122) || // a-z
        (x > 47 && x < 58)) {  // 0-9
      j += 1; // reward
    } else if ( x == 32 ) {
      j += 2; // extra reward for " "
    } else {
      j -= 1; // punishment
    }
  }

  return j;
}
std::string crackxor(const std::string& s) {

  std::string key;

  for(char c = 0; c < 127; c++) { // assume only ASCII
    std::string k = std::string(1, c); // cast char to string
    std::string res = xor_string(s, k);
    if(allprintable(res)) { // only printable ascii
      if (score(res) > res.length() * 0.9) { // 90% in 'A-Za-z0-9 '
        key = k;
        break;
      }
    }
  }
  return key;
}

int countbits(const std::string& x) {
  int ham = 0;
  for(int i = 0; i < x.length(); i++) {
    int c = x[i];
    ham += (0x80 & c) >> 7;
    ham += (0x40 & c) >> 6;
    ham += (0x20 & c) >> 5;
    ham += (0x10 & c) >> 4;
    ham += (0x08 & c) >> 3;
    ham += (0x04 & c) >> 2;
    ham += (0x02 & c) >> 1;
    ham += (0x01 & c);
  }
  return ham;
  }

int getham(const std::string& a, const std::string& b){
  return countbits(xor_string(a, b));
}

int guesskeylength(const std::string& s, int maxlength){
  float mindist = 255; // enough?
  int keylength = 0;

  for (int i = 2; i < maxlength; i++) {
    float ham = 0.0;
    int offset = 64;
    int offsets = 16;
    for(int j = 0; j < (offset * offsets); j += offset) {
      std::string a = s.substr(j, i);
      std::string b = s.substr(j + i, i);
      ham += getham(a, b) / (float)i;
    }
    if ((ham / offsets) < mindist) {
      mindist = ham / offsets;
      keylength = i;
    }
  }
  return keylength;
}

std::string padme(const std::string& s, int length){
  char pad = length - s.length();
  std::string padding;
  for(int i = 0; i < pad; i++) {
    padding += pad;
  }
  return s + padding;
}

std::string initialiseiv(int size, bool randomise){
  std::string iv;

  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> dist(0, 128);

  for(int i = 0; i < size; i++){
    iv += (char) randomise ? dist(mt): 0;
  }
  return iv;
}

bool ecbmatch(std::string hex){

  std::map <std::string,int> m;
  for(int i = 0; i < hex.length(); i += 32){
    std::string chomp = (hex.substr(i, 32));
    if (m.count(chomp)) {
      return true;
    } else {
      m[chomp] = 1;
    }
  }
  return false;
}

std::string padrand(int min, int max){
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> dist(min, max);

  return initialiseiv(dist(mt), true);
}

std::string encodev(const std::vector<std::string> v, const char join) {
  std::string s;

  for(int i = 0; i < v.size(); i++) {
    if ( i > 0 ) {
      s += join;
    }
    s += v[i];
  }

  return s;
}

const std::vector<std::string> explode(const std::string& s, const char& c) {
  std::string buff{""};
  std::vector<std::string> v;

  for(auto n:s)
  {
    if(n != c) buff+=n; else
    if(n == c && buff != "") { v.push_back(buff); buff = ""; }
  }
  if(buff != "") v.push_back(buff);

  return v;
}

std::vector<std::string> parsekv(const std::string& s, const char split){
  return explode(s, split);
}
