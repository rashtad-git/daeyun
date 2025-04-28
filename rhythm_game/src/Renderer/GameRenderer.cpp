#include "GameRenderer.h"

#include <vector>

#include "../Models/Math.h"

#include "./Screens/GameScreen.h"
#include "./Screens/MetronomeScreen.h"
#include "./Screens/ScoreScreen.h"

struct GameRenderer::PrivateData {
  std::vector<ScreenBase*> screens;
};

GameRenderer::GameRenderer() {
  This = new PrivateData();
  This->screens.push_back(new GameScreen());
  This->screens.push_back(new ScoreScreen());
  This->screens.push_back(new MetronomeScreen());

  for (auto screen : This->screens) {
    screen->Init();
  }
}

GameRenderer::~GameRenderer() {
  for (auto& screen : This->screens) {
    delete screen;
  }

  This->screens.clear();
  delete This;
}

void GameRenderer::PreRender() {
  for (auto screen : This->screens) {
    screen->PreRender();
  }
}

char GameRenderer::OnRender(const Point& p) {
  for (auto screen : This->screens) {
    if (screen->Contains(p)) {
      return screen->GetBoard(p - screen->GetPosition());
    }
  }

  return ' ';
}

void GameRenderer::PostRender() {
  for (auto screen : This->screens) {
    screen->PostRender();
  }
}
