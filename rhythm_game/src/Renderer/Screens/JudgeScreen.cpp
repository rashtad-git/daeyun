#include "JudgeScreen.h"

#include "../../System/DataManager.h"

JudgeScreen::JudgeScreen() {
  SetPosition(Point(23, 2));
  SetSize(Size(14, 11));
}

JudgeScreen::~JudgeScreen() {}

void JudgeScreen::Init() {
  DrawString(0, 0, "PERFECT");
  DrawString(0, 1, "GREAT");
  DrawString(0, 2, "GOOD");
  DrawString(0, 3, "BAD");
}

void JudgeScreen::PreRender() {
  const auto& game = DataManager::GetInstance().game;

  const int JudgeScaleToMilli = 2000;
  DrawNumber(8, 0, game.GetJudge(ScoreTypes::Perfect) * JudgeScaleToMilli, 3);
  DrawNumber(8, 1, game.GetJudge(ScoreTypes::Great) * JudgeScaleToMilli, 3);
  DrawNumber(8, 2, game.GetJudge(ScoreTypes::Good) * JudgeScaleToMilli, 3);
  DrawNumber(8, 3, game.GetJudge(ScoreTypes::Bad) * JudgeScaleToMilli, 3);

  for (int i = 0; i < 4; i++) {
    DrawString(12, i, "ms");
  }
}

void JudgeScreen::PostRender() {}
