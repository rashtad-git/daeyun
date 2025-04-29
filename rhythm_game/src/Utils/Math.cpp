#include "Math.h"

#include <random>

int Math::GetRandom(int min, int max, const std::set<int>& exclude) {
  static std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<> dist(min, max);
  int num = dist(gen);

  if (max - min <= (int)exclude.size()) {
    // warning
    return num;
  }

  while (exclude.find(num) != exclude.end()) {
    num = ((num + 1) % max) + min;
  }

  if (exclude.find(num) != exclude.end()) {
    num++;
  }

  return num;
}
