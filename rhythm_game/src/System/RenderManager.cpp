#include "RenderManager.h"
#include <Windows.h>
#include <cstdio>
#include <iostream>
#include <string>

#include "../Game/Config.h"
#include "DataManager.h"

#include "../Models/Math.h"

#include "../Renderer/GameRenderer.h"
#include "../Renderer/ResultRenderer.h"
#include "../Renderer/TitleRenderer.h"

const int MAX_LINES = 4;

struct RenderManager::PrivateData {
  GameRenderer gameRenderer;
  TitleRenderer titleRenderer;
  ResultRenderer resultRenderer;
};

RenderManager::RenderManager() {
  This = new PrivateData();
}

RenderManager::~RenderManager() {
  delete This;
}

void RenderManager::Render() const {
  auto& data = DataManager::GetInstance();

  IRenderer* renderer = nullptr;
  switch (data.game.GameState) {
    case GameState::Game:
      renderer = &This->gameRenderer;
      break;
    case GameState::Title:
      renderer = &This->titleRenderer;
      break;
    case GameState::Result:
      renderer = &This->resultRenderer;
      break;
    default:
      renderer = nullptr;
      break;
  }

  if (renderer != nullptr) {
    renderer->PreRender();
  }

  auto buffer = OnRender(renderer);

  // 커서 깜빡임을 방지하기 위한 위치 초기화 (Windows 환경에서만 사용)
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD cursorPos = {0, 0};
  SetConsoleCursorPosition(hConsole, cursorPos);

  std::cout << buffer.str();

  if (renderer != nullptr) {
    renderer->PostRender();
  }
}

std::stringstream RenderManager::OnRender(IRenderer* renderer) const {
  auto& data = DataManager::GetInstance();

  std::stringstream buffer;
  for (int y = 0; y < Config::SCREEN_HEIGHT; y++) {
    for (int x = 0; x < Config::SCREEN_WIDTH; x++) {
      if (y == 0 || y == Config::SCREEN_HEIGHT - 1 || x == 0 ||
          x == Config::SCREEN_WIDTH - 1) {
        // guide line
        buffer << "#";
      } else {
        if (renderer != nullptr) {
          buffer << renderer->OnRender(Point(x, y));
        } else {
          buffer << ' ';
        }
      }
    }

    if (y == 1) {
      char fpsBuffer[32];
      sprintf_s(fpsBuffer, sizeof(fpsBuffer), "   %.2f FPS",
                data.system.CurrentFPS);
      buffer << fpsBuffer;
    }

    if (y == 2) {
      buffer << "   " << data.system.UpdateFrame << " UpdateFrame";
    }

    if (y == 4) {
      buffer << "   " << "Speed " << data.game.NoteSpeed;
    }

    if (y == 5) {
      buffer << "   " << "BPM " << data.game.TimeSig.BPM;
    }

    if (y == 6) {
      buffer << "   " << "Meter " << data.game.TimeSig.Top << "/"
             << data.game.TimeSig.Bottom;
    }

    if (y == 8) {
      int count = 0;
      for (auto iter : data.user.scores)
        count += iter.second;

      int judge = count == 0 ? 0 : (int)((data.game.debugJudge / count) * 1000);
      buffer << "   " << "Judge " << judge << " ms         ";
    }

    buffer << std::endl;
  }

  return buffer;
}
