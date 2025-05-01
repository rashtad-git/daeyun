#include "ResultRenderer.h"

#include <vector>

#include "./Screens/GameScreen.h"
#include "./Screens/IndicatorScreen.h"
#include "./Screens/MetronomeScreen.h"
#include "./Screens/ScoreScreen.h"

struct ResultRenderer::PrivateData {
  std::vector<ScreenBase*> screens;
};

ResultRenderer::ResultRenderer() {
  This = new PrivateData();
  This->screens.push_back(new ScoreScreen());
}

ResultRenderer::~ResultRenderer() {
  delete This;
}

void ResultRenderer::Init() {
  for (auto screen : This->screens) {
    screen->Init();
  }
}

void ResultRenderer::PreRender() {
  for (auto screen : This->screens) {
    screen->PreRender();
  }
}

char ResultRenderer::OnRender(const Point& p) {
  for (auto screen : This->screens) {
    if (screen->Contains(p)) {
      return screen->GetBoard(p - screen->GetPosition());
    }
  }

  return ' ';
}

void ResultRenderer::PostRender() {
  for (auto screen : This->screens) {
    screen->PreRender();
  }
}
