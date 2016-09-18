#include <sstream>  // for stringstream
#include <iomanip>  // for setw and setfill
#include <iostream> // for cout
#include <map>
#include <random>
#include <vector>

std::string hex2str(const std::string& h) {
  std::string s;

  if(h.length() & 1) {
    throw("Tried to string a non hex string");
  }

  for(size_t i = 0; i < h.length(); i += 2) {
    int j = 0;
    std::stringstream ss(h.substr(i,2));
    ss >> std::hex >> j;
    s += char(j);
  }
  return s;
}

std::string v2str(const std::vector<uint8_t>& v) {
  std::string s;
  for(size_t i = 0; i < v.size(); i++) {
    s += v[i];
  }
  return s;
}

std::vector<uint8_t> hex2v(const std::vector<uint8_t>& h) {
  std::vector<uint8_t> v;

  if(h.size() & 1) {
    throw("Tried to string a non hex string");
  }

  for(size_t i = 0; i < h.size(); i += 2) {
    int j = 0;
    std::string s(1, h[i]);  // there must be a better way to do this?!
    s += h[i+1];
    std::stringstream ss(s);
    ss >> std::hex >> j;
    v.push_back((uint8_t)j);
  }
  return v;
}

std::vector<uint8_t> xor_vector(const std::vector<uint8_t>& v1, const std::vector<uint8_t>& v2) {

  std::vector<uint8_t> v;

  for(size_t i = 0; i < v1.size(); i++) {
    v.push_back(v1[i] ^ v2[i % v2.size()]);
  }

  return v;
}

std::string str2hex(const std::string& s) {
  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  for (size_t i = 0; s.length() > i; ++i) {
      ss << std::setw(2) << static_cast<unsigned int>(static_cast<unsigned char>(s[i]));
  }
  return ss.str();
}

std::string v2hex(const std::vector<uint8_t>& v) {
  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  for (size_t i = 0; v.size() > i; ++i) {
      ss << std::setw(2) << static_cast<unsigned int>(v[i]); // cast to int to avoid char
  }
  return ss.str();
}

bool allprintable(const std::vector<uint8_t>& v) {
  for(size_t i = 0; i < v.size(); i++) {
    if(v[i] > 127) {
      return false; // anything non-ASCII
    }
  }
  return true;
}

int score(const std::vector<uint8_t>& v) {
  int j = 0;
  for(size_t i = 0; i < v.size(); i++) {
    int x = v[i];
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

std::vector<uint8_t> crackxor(const std::vector<uint8_t>& v) { // TODO: return char?
  std::vector<uint8_t> key;

  for(uint8_t c = 0; c < 127; c++) {
    std::vector<uint8_t> k({c});
    std::vector<uint8_t> res = xor_vector(v, k);
    if(allprintable(res)) { // only printable ASCII
      if (score(res) > res.size() * 0.9) { // 90% in 'A-Za-z0-9 '
        key = k;
        break;
      }
    }
  }
  return key;
}

int countbits(const std::vector<uint8_t>& v) {
  int ham = 0;
  for(size_t i = 0; i < v.size(); i++) {
    int c = v[i];
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

int getham(const std::vector<uint8_t>& v1, const std::vector<uint8_t>& v2) {
  return countbits(xor_vector(v1, v2));
}

int guesskeylength(const std::vector<uint8_t>& v, int maxlength) {
  float mindist = 255; // enough?
  int keylength = 0;

  for (int i = 1; i < maxlength; i++) { // key length > 0
    float ham = 0.0;
    int offset = 64;
    int offsets = 16;
    for(int j = 0; j < (offset * offsets); j += offset) {
      std::vector<uint8_t> v1(&v[j],&v[j + i]);
      std::vector<uint8_t> v2(&v[j + i],&v[j + 2*i]);
      ham += getham(v1, v2) / (float)i;
    }
    if ((ham / offsets) < mindist) {
      mindist = ham / offsets;
      keylength = i;
    }
  }
  return keylength;
}

std::vector<uint8_t> padme(const std::vector<uint8_t>& v, int length){
  uint8_t pad = length - v.size();
  std::vector<uint8_t> padding = v;

  for(int i = 0; i < pad; i++) {
    padding.push_back(pad);
  }

  return padding;
}

std::vector<uint8_t> initialiseiv(uint8_t length, bool randomise){
  std::vector<uint8_t> iv;

  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> dist(0, 127); // can we go up to 256 now?

  for(uint8_t i = 0; i < length; i++){
    iv.push_back(randomise ? uint8_t(dist(mt)): 0);
  }
  return iv;
}

bool ecbmatch(std::vector<uint8_t> v){

  const size_t KEY_LENGTH = 16;

  std::map <std::vector<uint8_t>, int> m;

  for(size_t i = 0; i < v.size(); i += KEY_LENGTH){
    std::vector<uint8_t> chomp = std::vector<uint8_t>(&v[i], &v[i+KEY_LENGTH]);
    if (m.count(chomp)) {
      return true;
    } else {
      m[chomp] = 1;
    }
  }
  return false;
}

std::vector<uint8_t> padrand(int min, int max) {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> dist(min, max);

  return initialiseiv(dist(mt), true);
}

std::string encodev(const std::vector<std::string> v, const char join) {
  std::string s;

  for(size_t i = 0; i < v.size(); i++) {
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
