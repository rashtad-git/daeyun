#include "TitleScreen.h"

#include "../../Game/Config.h"
#include "../../System/DataManager.h"
#include "../../Utils/Math.h"

TitleScreen::TitleScreen() {
  SetPosition(Point(40, 15));

  SetSize(Size(36, 20));
}

TitleScreen::~TitleScreen() {}

void TitleScreen::Init() {
  auto& game = DataManager::GetInstance().game;

  DrawString(2, 1, " ---------    Options   --------- ");
  DrawString(2, 2, "|   Difficulty   [   VeryHard ]  |");
  DrawString(2, 3, "|          BPM   [        120 ]  |");
  DrawString(2, 4, "|    PlaySound   [         On ]  |");
  DrawString(2, 5, " -------------------------------- ");
}

void TitleScreen::PreRender() {
  auto& game = DataManager::GetInstance().game;

  switch (game.Difficulty) {
    case Difficulty::Beginner:
      DrawString(21, 2, "  Beginner ");
      break;
    case Difficulty::Easy:
      DrawString(21, 2, "      Easy ");
      break;
    case Difficulty::Normal:
      DrawString(21, 2, "    Normal ");
      break;
    case Difficulty::Hard:
      DrawString(21, 2, "      Hard ");
      break;
    case Difficulty::VeryHard:
      DrawString(21, 2, "  VeryHard ");
      break;
    case Difficulty::Expert:
      DrawString(21, 2, "    Expert ");
      break;
    case Difficulty::Master:
      DrawString(21, 2, "    Master ");
      break;
  }

  DrawNumber(27, 3, game.TimeSig.BPM, 4);
  DrawString(27, 4, game.IsPlaySound ? "  On" : " Off", 5);
}

void TitleScreen::PostRender() {}
