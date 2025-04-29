#pragma once

#include <Windows.h>
#include <list>
#include <map>

#include "../Game/Config.h"

#include "ButtonState.h"

enum class ScoreTypes {
  None,
  Miss,
  Bad,
  Good,
  Great,
  Perfect,
};

enum class InputControl {
  None,
  Left,
  Right,
  Up,
  Down,
  Enter,
  Escape,

  Button_1,
  Button_2,
  Button_3,
  Button_4,
};

const InputControl ActionButtons[] = {
    InputControl::Button_1,
    InputControl::Button_2,
    InputControl::Button_3,
    InputControl::Button_4,
};

struct UserData {
 public:
  std::map<InputControl, ButtonState> input;
  std::map<ScoreTypes, int> scores;
  std::list<double> fastIndicator;
  std::list<double> lateIndicator;

  UserData() {
    input.insert({InputControl::Left, VK_LEFT});
    input.insert({InputControl::Right, VK_RIGHT});
    input.insert({InputControl::Up, VK_UP});
    input.insert({InputControl::Down, VK_DOWN});
    input.insert({InputControl::Enter, VK_RETURN});
    input.insert({InputControl::Escape, VK_ESCAPE});
    input.insert({InputControl::Button_1, 'D'});
    input.insert({InputControl::Button_2, 'F'});
    input.insert({InputControl::Button_3, 'J'});
    input.insert({InputControl::Button_4, 'K'});
  }
};
