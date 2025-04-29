#include "TitleScreen.h"

#include "../../Game/Config.h"
#include "../../System/DataManager.h"
#include "../../Utils/Math.h"

TitleScreen::TitleScreen() {
  SetPosition(Point(40, 15));

  SetSize(Size(26, 20));
}

TitleScreen::~TitleScreen() {}

void TitleScreen::Init() {
  auto& game = DataManager::GetInstance().game;

  DrawString(2, 1, " -----   Options  ----- ");
  DrawString(2, 2, "|Difficulty            |");
  DrawString(2, 3, "|       BPM            |");
  DrawString(2, 4, "| PlaySound            |");
  DrawString(2, 5, " ---------------------- ");

  // DrawString(2, 3, "| NoteSpeed            |");
  // DrawString(2, 4, "|     Meter            |");
}

void TitleScreen::PreRender() {}

void TitleScreen::PostRender() {}
