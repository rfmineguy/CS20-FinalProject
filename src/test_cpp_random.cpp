#include <random>
#include <iostream>

int random_int() {
  std::random_device rd;
  std::uniform_int_distribution<int> dist(1, 9);
  return dist(rd);
}

int main() {
  for (int i = 0; i < 25; i++) {
    std::cout << random_int() << std::endl;
  }
}