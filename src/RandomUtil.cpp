#include "../include/util/RandomUtil.hpp"

RandomInt::RandomInt() {}
RandomInt::~RandomInt() {}
int RandomInt::GetRandomInt(int lower, int upper) {
  std::uniform_int_distribution<int> dist(lower, upper);
  return dist(rd);
}