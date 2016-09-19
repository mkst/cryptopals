#include "mersenne.h"

#include <random>
#include <cassert>

void challenge_21(){

  Mersenne m(5489);
  std::mt19937 mt_rand(5489);

  for(int i = 0; i < 1000; i++) {
    assert(mt_rand() == m.extract_number());
  }

}
