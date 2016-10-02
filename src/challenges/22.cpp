#include <iostream>
#include <chrono>

#include "mersenne.h"
#include "utils.h"

void challenge_22() {

  Mersenne r(123);

  // sleep before seed generation
  int nap_time = 5 + (r.extract_number() % 5);
  sleep(nap_time);

  // get seed from 'now'
  std::chrono::system_clock::time_point n = std::chrono::system_clock::now();
  uint64_t seed = std::chrono::duration_cast<std::chrono::milliseconds>(n.time_since_epoch()).count();
  Mersenne r2(seed);

  // sleep after seed generated
  nap_time = 5 + (r.extract_number() % 5);
  sleep(nap_time);

  // get first number from our mersenne twister
  uint32_t first = r2.extract_number();

  // now bruteforce crack from 'now'
  n = std::chrono::system_clock::now();
  uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(n.time_since_epoch()).count();

  while(true) {
    Mersenne g(now);
    if(g.extract_number() == first) {
      std::cout << "seed used: " << now << std::endl;
      break;
    }
    now--;
  }
}
