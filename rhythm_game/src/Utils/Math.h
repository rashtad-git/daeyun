#pragma once

#include <set>

class Math
{
public:
	static int GetRandom(int min, int max, std::set<int> exclude = std::set<int>());
};
