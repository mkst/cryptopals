#include <string> // for string

static const std::string lut = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

std::string b64encode(const std::string& s){

  std::string output;
  int value;

  int leftovers  = s.length() % 3;
  int length = s.length();
  int i = 0;

  // example
  // 11001101 10100110 00111001
  // xx110011 xx011010 xx011000 xx111001
  // 00110011 00011010 00011000 00111001
  while(i < length) {
    if(i + 3 <= length) {
      // FC = 11111100, mask off bottom 2 bits and move right 2
      value = (0xFC & (int)s[i]) >> 2;
      output += lut[value];
      // 03 = 00000011, mask off top 6 bits and move left 4
      value = (0x03 & (int)s[i]) << 4;
      // F0 = 11110000, mask off top 4 bits and move right 4
      value |= (0xF0 & (int)s[i+1]) >> 4;
      output += lut[value];
      // 0F = 00001111, mask off bottom 4 bits and move left 2
      value = (0x0F & (int)s[i+1]) << 2;
      // C0 = 1100000, mask off top 2 bits and move right 6
      value |= (0xC0 & (int)s[i+2]) >> 6;
      output += lut[value];
      // 3F = 00111111, mask off final 6 bits
      value = (0x3F & (int)s[i+2]);
      output += lut[value];
    } else {
      if (leftovers == 1){
        value = (0xFC & (int)s[i]) >> 2;
        output += lut[value];
        value = (0x03 & (int)s[i]) << 4;
        output += lut[value];
        output += "==";
      }
      else {
        value = (0xFC & (int)s[i]) >> 2;
        output += lut[value];
        value = (0x03 & (int)s[i]) << 4;
        value |= (0xF0 & (int)s[i+1]) >> 4;
        output += lut[value];
        value = (0x0F & (int)s[i+1]) << 2;
        output += lut[value];
        output += "=";
      }
    }
    i += 3;
  }
  return output;
}

std::string b64decode(const std::string& d) {
  std::string decoded;

  if(d.length() % 4) {
    throw("Tried to decode non base64 string");
  }
  int b[4];
  int j = 0;
  for(int i = 0; i < d.length(); i += 4){
    b[0] = lut.find(d[i]);
    b[1] = lut.find(d[i+1]);
    b[2] = lut.find(d[i+2]);
    b[3] = lut.find(d[i+3]);
    decoded += ((b[0] << 2) | (b[1] >> 4));
    if (b[2] < 64)      {
        decoded += ((b[1] << 4) | (b[2] >> 2));
        if (b[3] < 64)  {
            decoded += ((b[2] << 6) | b[3]);
        }
    }
  }
  return decoded;
}
