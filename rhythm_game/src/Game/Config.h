#pragma once

struct Config {
  static const int SCREEN_WIDTH = 80;
  static const int SCREEN_HEIGHT = 24;

  static const int BUTTON_COUNT = 4;

  static const int GAME_LINE_HEIGHT = 19;
  static int GetPerfectJudge() { return GAME_LINE_HEIGHT - 3; }
  static int GetJudgeEnd() { return GAME_LINE_HEIGHT + 2; }

  static double GetIndicatorDuration() { return 120; }
};

enum class ScoreTypes {
  None,
  Miss,
  Bad,
  Good,
  Great,
  Perfect,
};

const double StandardJudge_Perfect = 0.01;
const double StandardJudge_Great = 0.02;
const double StandardJudge_Good = 0.04;
const double StandardJudge_Bad = 0.08;

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
