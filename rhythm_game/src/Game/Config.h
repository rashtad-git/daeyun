#pragma once

struct Config {
  static const int SCREEN_WIDTH = 80;
  static const int SCREEN_HEIGHT = 24;

  static const int BUTTON_COUNT = 4;

  static const int GAME_LINE_HEIGHT = 19;
  static int GetPerfectJudge() { return GAME_LINE_HEIGHT - 3; }
  static int GetJudgeEnd() { return GAME_LINE_HEIGHT + 2; }
};
