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

#include "../Controller/Node/Node.h"

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

void RenderManager::Init() {
  This->gameRenderer.Init();
  This->titleRenderer.Init();
  This->resultRenderer.Init();
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

    buffer << "   ";

    if (y == 1) {
      char fpsBuffer[32];
      sprintf_s(fpsBuffer, sizeof(fpsBuffer), "%.2f FPS",
                data.system.CurrentFPS);
      buffer << fpsBuffer;
    }

    if (y == 2) {
      buffer << data.system.UpdateFrame << " UpdateFrame";
    }

    if (y == 5) {
      buffer << "BPM " << data.game.BeatInfo.BPM << "     ";
    }

    if (y == 6) {
      buffer << "Difficulty ";
      switch (data.game.Difficulty) {
        case Difficulty::Beginner:
          buffer << "Beginner";
          break;
        case Difficulty::Easy:
          buffer << "Easy";
          break;
        case Difficulty::Normal:
          buffer << "Normal";
          break;
        case Difficulty::Hard:
          buffer << "Hard";
          break;
        case Difficulty::VeryHard:
          buffer << "Very Hard";
          break;
        case Difficulty::Expert:
          buffer << "Expert";
          break;
        case Difficulty::Master:
          buffer << "Master";
          break;
      }
    }

    DebugPrint(buffer, y);

    buffer << std::endl;
  }

  return buffer;
}

const Node* GetFirstNode() {
  auto& game = DataManager::GetInstance().game;
  for (const auto& node : game.StageNodes) {
    if (node->IsActive() == false || node->HasEffect())
      continue;

    return node;
  }

  return nullptr;
}

void RenderManager::DebugPrint(std::stringstream& buffer, int y) const {
  auto& data = DataManager::GetInstance();

  if (data.system.showDebug == false) {
    return;
  }

  if (y == 8) {
    const auto& node = GetFirstNode();
    if (node != nullptr) {
      buffer << "Line " << node->GetLine();
    }
  }

  if (y == 9) {
    const auto& node = GetFirstNode();
    if (node != nullptr) {
      buffer << "Index " << node->GetIndex() << " ("
             << node->GetIndex() - Config::GetPerfectJudge() << ")   ";
    }
  }

  if (y == 10) {
    const auto& node = GetFirstNode();
    if (node != nullptr) {
      char durationBuffer[32];
      sprintf_s(durationBuffer, sizeof(durationBuffer), "%.3f",
                node->GetTimeToJudgmentLine());
      buffer << "Duration " << durationBuffer << "     ";
    }
  }

  if (y == 11) {
    const auto& node = GetFirstNode();
    if (node != nullptr) {
      char elapsedBuffer[32];
      sprintf_s(elapsedBuffer, sizeof(elapsedBuffer), "%.3f",
                node->GetElapsed());
      buffer << "Elapsed " << elapsedBuffer << "     ";
    }
  }

  if (y == 13) {
    char frameBuffer[32];
    sprintf_s(frameBuffer, sizeof(frameBuffer), "%.3f",
              data.game.currentBeatFrame);
    buffer << "BeatFrame " << frameBuffer << " / "
           << data.game.BeatInfo.GetBeatInterval() << "    ";
  }

  if (y == 14) {
    buffer << "BeatIndex " << data.game.DebugBeatIndex << " / "
           << data.game.BeatInfo.GetTotalBeats() << "    ";
  }

  if (y == 15) {
    buffer << "NodeCount " << data.game.DebugNodeCount << "    ";
  }
}
