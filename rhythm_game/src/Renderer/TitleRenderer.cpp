#include "TitleRenderer.h"

#include <vector>

#include "../Models/Math.h"

#include "./Screens/GameScreen.h"
#include "./Screens/JudgeScreen.h"
#include "./Screens/MetronomeScreen.h"
#include "./Screens/OptionScreen.h"

struct TitleRenderer::PrivateData {
  std::vector<ScreenBase*> screens;
};

TitleRenderer::TitleRenderer() {
  This = new PrivateData();
  This->screens.push_back(new MetronomeScreen());
  This->screens.push_back(new OptionScreen());
  This->screens.push_back(new GameScreen());
  This->screens.push_back(new JudgeScreen());
}

TitleRenderer::~TitleRenderer() {
  for (auto& screen : This->screens) {
    delete screen;
  }

  This->screens.clear();
  delete This;
}

void TitleRenderer::Init() {
  for (auto screen : This->screens) {
    screen->Init();
  }
}

void TitleRenderer::PreRender() {
  for (auto screen : This->screens) {
    screen->PreRender();
  }
}

char TitleRenderer::OnRender(const Point& p) {
  for (auto screen : This->screens) {
    if (screen->Contains(p)) {
      return screen->GetBoard(p - screen->GetPosition());
    }
  }

  return ' ';
}

void TitleRenderer::PostRender() {
  for (auto screen : This->screens) {
    screen->PreRender();
  }
}
