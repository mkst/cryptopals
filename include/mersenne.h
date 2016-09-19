#include <iostream>
#include <cstdint>

class Mersenne {

  static const uint32_t w = 32;
  static const int64_t  n = 624;
  static const uint32_t m = 397;
  static const uint32_t r = 31;

  static const uint32_t a = 0x9908B0DF;
  static const uint32_t u = 11;
  static const uint32_t d = 0xFFFFFFFF;
  static const uint32_t s = 7;
  static const uint32_t b = 0x9D2C5680;
  static const uint32_t t = 15;
  static const uint32_t c = 0xEFC60000;

  static const uint32_t l = 18;
  static const uint32_t f = 1812433253 ;

  int64_t index = 0;
  uint64_t MT[n];

  uint64_t lower_mask = 0x7FFFFFFF;
  uint64_t upper_mask = 0x80000000;

  public:
    Mersenne(uint64_t seed);
    uint32_t extract_number();
    void twist();
};
