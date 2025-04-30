#include "IndicatorScreen.h"

#include <cmath>

#include "System/DataManager.h"

const int INDICATOR_WIDTH = 49;

IndicatorScreen::IndicatorScreen() {
  SetPosition(Point(27, 16));

  SetSize(Size(INDICATOR_WIDTH + 2, 7));
}

IndicatorScreen::~IndicatorScreen() {}

void IndicatorScreen::Init() {

  prevCount = 0;
  judges.empty();

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
      c = '_';  // bad
    } else if (i < great_negative) {
      c = '~';  // good
    } else if (i < perfect_negative) {
      c = '=';  // great
    } else if (i < perfect_positive) {
      c = '^';  // perfect
    } else if (i < great_positive) {
      c = '=';  // great
    } else if (i < good_positive) {
      c = '~';  // good
    } else {
      c = '_';  // bad
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
  while (prevCount < count) {
    double judge = user.indicator[prevCount];

    int point = IndicateToPoint(judge);
    judgeCount[point]++;
    judgePoints.push(std::make_pair(game.GameTime, point));

    prevCount++;
  }
}

void IndicatorScreen::PostRender() {}

int IndicatorScreen::IndicateToPoint(double indicate) {
  auto& game = DataManager::GetInstance().game;

  int half = (INDICATOR_WIDTH - 1) * 0.5;
  double percent = std::abs(indicate) / game.Judge_Bad;
  int point = (int)(half * percent);
  if (indicate < 0) {
    point = half - point;
  } else {
    point = half + point;
  }

  return point;
}
