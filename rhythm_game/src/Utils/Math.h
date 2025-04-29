#pragma once

#include <set>

class Math {
 public:
  static int GetRandom(int min, int max, const std::set<int>& exclude = {});
};
