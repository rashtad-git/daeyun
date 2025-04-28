#pragma once

#include <map>
#include <list>

#include "../Game/Config.h"

#include "ButtonState.h"

enum class ScoreTypes
{
	None,
	Miss,
	Bad,
	Good,
	Great,
	Perfect,
};

struct UserData
{
public:
	ButtonState input[Config::BUTTON_COUNT] = { 'D', 'F', 'J', 'K' };
	std::map<ScoreTypes, int> scores;
	std::list<double> fastIndicator;
	std::list<double> lateIndicator;
};
