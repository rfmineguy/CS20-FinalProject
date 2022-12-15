#ifndef RANDOM_UTIL_HPP
#define RANDOM_UTIL_HPP
#include <random>

class RandomInt {
public:
  RandomInt();
  ~RandomInt();
  int GetRandomInt(int lower, int upper);
  
private:
  std::random_device rd;
};

#endif