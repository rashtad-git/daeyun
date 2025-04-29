#include "GameScreen.h"

#include "../../Controller/Node/Node.h"
#include "../../Game/Config.h"
#include "../../System/DataManager.h"

const Point GameScreen_Position(3, 2);

GameScreen::GameScreen() {
  SetPosition(Point(3, 2));

  //// size width
  // total line = 4
  // line spacing = 3
  // line split = 1 * 3
  // edge 2

  SetSize(Size(3 * 4 + 3 + 2, Config::GAME_LINE_HEIGHT + 2));
}

GameScreen::~GameScreen() {}

void GameScreen::Init() {
  const Size& size = GetSize();
  auto& user = DataManager::GetInstance().user;

  // default fields
  for (int x = 0; x < size.width; x++) {
    for (int y = 0; y < size.height; y++) {
      Point p(x, y);

      // cleanup
      SetBoard(p, ' ');

      if (x == 0) {
        // left side, skip bottom line
        if (y != size.height - 1) {
          SetBoard(p, ']');
        }
      } else if (x == size.width - 1) {
        // right side, skip bottom line
        if (y != size.height - 1) {
          SetBoard(p, '[');
        }
      } else if (x % 4 == 0) {
        // spacing, skip bottom line
        if (y != size.height - 1) {
          SetBoard(p, '|');
        }
      } else if (x % 4 == 2) {
        // Button
        int line = x / 4;
        if (y == size.height - 2) {
          SetBoard(p, user.input.at(ActionButtons[line]).key);
        }
      } else if ((x % 4 == 1 || x % 4 == 3) && y == size.height - 5) {
        // Judgment line
        SetBoard(p, '=');
      }
    }
  }
}

void GameScreen::PreRender() {
  DrawTapped();
  DrawNode();
}

void GameScreen::PostRender() {}

void GameScreen::DrawTapped() {
  auto& size = GetSize();
  auto& user = DataManager::GetInstance().user;
  for (int i = 0; i < Config::BUTTON_COUNT; i++) {
    const auto& button = user.input.find(ActionButtons[i]);
    if (button == user.input.end())
      continue;

    int x = (i + 1) * 4 - 2;
    int y = size.height - 1;
    Point p(x, y);
    if (button->second.effectFrame > 0) {
      if (button->second.effectFrame < 0.15) {
        SetBoard(p, '*');
      } else {
        SetBoard(p, '+');
      }
    } else if (button->second.pressed) {
      SetBoard(p, '-');
    } else {
      SetBoard(p, ' ');
    }
  }
}

void GameScreen::DrawNode() {
  auto& size = GetSize();
  auto& game = DataManager::GetInstance().game;

  for (int i = 0; i < 4; ++i) {
    for (int y = 0; y < size.height - 2; y++) {
      Point p(2 + i * 4, y);
      SetBoard(p, ' ');
    }
  }

  for (auto node : game.StageNodes) {
    if (node->IsActive() == false)
      continue;

    int line = node->GetLine();
    int index = node->GetIndex();

    if (index > Config::GetJudgeEnd())
      continue;

    int perfectJudge = Config::GetPerfectJudge();
    int judge = index - perfectJudge;
    if (judge > 0)
      judge /= 2;

    Point p(2 + line * 4, perfectJudge + judge);
    SetBoard(p, node->GetGraphic());
  }
}
