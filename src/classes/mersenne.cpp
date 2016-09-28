#include "mersenne.h"

Mersenne::Mersenne(uint64_t seed){

  index = n;    // initialise index to 624
  MT[0] = seed;

  for (uint64_t i = 1; i < n; i++) { // initialise array
    MT[i] = 0xFFFFFFFF & (f * (MT[i-1] xor (MT[i-1] >> (w-2))) + i);
  }
}

uint32_t Mersenne::extract_number(){

  if(index >= n){
    twist();
  }

  uint64_t y = MT[index];

  y = y ^ (y >> u);
  y = y ^ ((y << s) & b);
  y = y ^ ((y << t) & c);
  y = y ^ (y >> l);

  index++;
  return 0xFFFFFFFF & y;
}

void Mersenne::twist() {

  for(int64_t i = 0; i < n; i++) {
    uint64_t y = 0xFFFFFFFF & ((MT[i] & upper_mask) +
                               (MT[(i+1) % n] & lower_mask));
    MT[i] = MT[(i + m) % n] ^ y >> 1;
    if(y % 2 != 0) {
      MT[i] = MT[i] ^ a;
    }
  }
  // reset index
  index = 0;
}
