#pragma once

#include <Windows.h>
#include <map>
#include <vector>

#include "../Game/Config.h"

#include "ButtonState.h"

struct UserData {
 public:
  std::map<InputControl, ButtonState> input;
  std::map<ScoreTypes, int> scores;

  std::vector<double> indicator;
  int fastCount;
  int slowCount;

  int comboCount;
  int maxComboCount;

  int optionFocus;

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

  const ButtonState* GetInput(InputControl control) const {
    auto iter = input.find(control);
    if (iter != input.end()) {
      return &iter->second;
    }

    return nullptr;
  }

  void Clear() {
    scores.clear();
    indicator.clear();
    fastCount = 0;
    slowCount = 0;
    comboCount = 0;
    maxComboCount = 0;
    optionFocus = 0;
  }
};
