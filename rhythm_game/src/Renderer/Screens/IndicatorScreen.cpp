#include "IndicatorScreen.h"

#include <cmath>

#include "../../System/DataManager.h"

const int INDICATOR_WIDTH = 49;

IndicatorScreen::IndicatorScreen() {
  SetPosition(Point(27, 16));

  SetSize(Size(INDICATOR_WIDTH + 2, 7));
}

IndicatorScreen::~IndicatorScreen() {}

void IndicatorScreen::Init() {

  prevCount = 0;
  judgePoints.empty();

  auto& game = DataManager::GetInstance().game;

  DrawString(0, 0, " ---- Indicator ---------------------------------- ");
  DrawString(0, 1, "|                                                 |");
  DrawString(0, 2, "|                                                 |");
  DrawString(0, 3, "|                                                 |");
  DrawString(0, 4, "|                                                 |");
  DrawString(0, 5, "|                                                 |");
  DrawString(0, 6, " ------------------------------------------------- ");

  game.Judge_Bad;

  int good_negative = IndicateToPoint(-game.Judge_Good);
  int great_negative = IndicateToPoint(-game.Judge_Great);
  int perfect_negative = IndicateToPoint(-game.Judge_Perfect);
  int perfect_positive = IndicateToPoint(game.Judge_Perfect);
  int great_positive = IndicateToPoint(game.Judge_Great);
  int good_positive = IndicateToPoint(game.Judge_Good);

  int barPosY = 4;
  int width = (INDICATOR_WIDTH - 1) / 2;
  for (int i = 0; i < width; i++) {

    char c = '-';
    if (i < good_negative) {
      c = '~';  // bad
    } else if (i < great_negative) {
      c = '-';  // good
    } else if (i < perfect_negative) {
      c = '=';  // great
    } else if (i < perfect_positive) {
      c = '+';  // perfect
    } else if (i < great_positive) {
      c = '=';  // great
    } else if (i < good_positive) {
      c = '-';  // good
    } else {
      c = '~';  // bad
    }

    DrawChar(1 + i, barPosY, c);
    DrawChar(INDICATOR_WIDTH - i, barPosY, c);
  }
  // center
  DrawString(1 + width, barPosY, "|");

  int indent = 2;
  DrawString(1 + indent, barPosY + 1, "fast");
  DrawString(INDICATOR_WIDTH - 4 - indent, barPosY + 1, "slow");
}

void IndicatorScreen::PreRender() {
  auto& game = DataManager::GetInstance().game;
  auto& user = DataManager::GetInstance().user;

  int count = user.indicator.size() - prevCount;
  for (int i = 0; i < count; i++) {
    double judge = user.indicator[prevCount + i];

    int point = IndicateToPoint(judge);
    judgeCount[point]++;
    judgePoints.push_back(std::make_pair(game.GameTime, point));
  }

  prevCount = user.indicator.size();

  const char judgeChars[] = {'_', '-', '=', '+', '^'};
  const int judgeCharSize = sizeof(judgeChars) / sizeof(judgeChars[0]);
  const int judgeCharHeight = 3;

  // clear
  for (int i = 0; i < INDICATOR_WIDTH; i++) {
    DrawChar(1 + i, 3, ' ');
    DrawChar(1 + i, 2, ' ');
    DrawChar(1 + i, 1, ' ');
  }

  for (auto iter : judgeCount) {
    int point = iter.first;
    int value = iter.second;

    if (value > judgeCharSize * judgeCharHeight) {
      value = judgeCharSize * judgeCharHeight;
    }

    int height = 0;
    while (value > 0) {
      char c = judgeChars[value % judgeCharSize];
      DrawChar(1 + point, 3 - height, c);
      height++;
      value -= judgeCharSize;
    }
  }
}

void IndicatorScreen::PostRender() {
  if (judgePoints.empty()) {
    return;
  }

  auto& game = DataManager::GetInstance().game;
  double limitTime = game.GameTime - 3;
  auto front = judgePoints.front();
  while (front.first < limitTime) {
    judgeCount[front.second]--;
    judgePoints.pop_front();
    if (judgePoints.empty()) {
      break;
    }

    front = judgePoints.front();
  }
}

int IndicatorScreen::IndicateToPoint(double indicate) {
  auto& game = DataManager::GetInstance().game;

  int half = (INDICATOR_WIDTH - 1) * 0.5;
  double percent = std::abs(indicate) / game.Judge_Bad;
  if (percent > 1)
    percent = 1;
  int point = (int)(half * percent);
  if (indicate < 0) {
    point = half - point;
  } else {
    point = half + point;
  }

  return point;
}
