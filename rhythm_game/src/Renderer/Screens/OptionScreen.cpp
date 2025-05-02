#include "OptionScreen.h"

#include "../../Game/Config.h"
#include "../../System/DataManager.h"
#include "../../Utils/Math.h"

OptionScreen::OptionScreen() {
  SetPosition(Point(24, 11));

  SetSize(Size(50, 20));
}

OptionScreen::~OptionScreen() {}

void OptionScreen::Init() {
  auto& game = DataManager::GetInstance().game;

  DrawString(2, 1, " --------------    Option   -------------- ");
  DrawString(2, 2, "|    Difficulty   [     VeryHard     ]    |");
  DrawString(2, 3, "|           BPM   [          120     ]    |");
  DrawString(2, 4, "|         Speed   [            1     ]    |");
  DrawString(2, 5, "|    JudgeScale   [          2.0     ]    |");
  DrawString(2, 6, "|     PlaySound   [           On     ]    |");
  DrawString(2, 7, "|     ShowDebug   [          Off     ]    |");
  DrawString(2, 8, "|                 [      Start       ]    |");
  DrawString(2, 9, " ----------------------------------------- ");
}

void OptionScreen::PreRender() {
  auto& data = DataManager::GetInstance();

  switch (data.game.Difficulty) {
    case Difficulty::Beginner:
      DrawString(24, 2, "  Beginner ");
      break;
    case Difficulty::Easy:
      DrawString(24, 2, "      Easy ");
      break;
    case Difficulty::Normal:
      DrawString(24, 2, "    Normal ");
      break;
    case Difficulty::Hard:
      DrawString(24, 2, "      Hard ");
      break;
    case Difficulty::VeryHard:
      DrawString(24, 2, "  VeryHard ");
      break;
    case Difficulty::Expert:
      DrawString(24, 2, "    Expert ");
      break;
    case Difficulty::Master:
      DrawString(24, 2, "    Master ");
      break;
  }

  DrawNumber(30, 3, data.game.BeatInfo.BPM, 4);
  DrawNumber(30, 4, data.game.NoteSpeed, 4);
  DrawDouble(30, 5, data.game.JudgeScale, 1);
  DrawString(30, 6, data.game.IsPlaySound ? "  On" : " Off", 5);
  DrawString(30, 7, data.system.showDebug ? "  On" : " Off", 5);

  // focus 표시
  for (int i = 0; i < 7; i++) {
    int y = 2 + i;

    if (data.user.optionFocus == i) {
      DrawString(22, y, ">>");
      DrawString(36, y, "<<");
    } else {
      DrawString(22, y, "  ");
      DrawString(36, y, "  ");
    }
  }
}

void OptionScreen::PostRender() {}
