#include "GameManager.h"
#include <ctime>

#include "DataManager.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "StateManager.h"

struct GameManager::PrivateData {
  InputManager inputManager;
  StateManager stateManager;
  RenderManager renderManager;

  bool isRunning;
  clock_t lastUpdateTime;
  clock_t lastRenderTime;
};

GameManager::GameManager() {
  This = new PrivateData();
  This->isRunning = true;
}

GameManager::~GameManager() {
  delete This;
}

void GameManager::run() {
  This->lastUpdateTime = clock();
  This->lastRenderTime = clock();

  This->stateManager.Init();
  This->renderManager.Init();

  while (This->isRunning) {
    if (onGameLoop() == false) {
      This->isRunning = false;
    }
  }
}

bool GameManager::onGameLoop() {
  auto& data = DataManager::GetInstance();
  auto now = clock();
  double deltaTime =
      static_cast<double>(now - This->lastUpdateTime) / CLOCKS_PER_SEC;
  data.system.UpdateFrame = deltaTime;

  This->inputManager.Update(deltaTime);
  This->stateManager.Update(deltaTime);

  if (data.system.isPlaying == false) {
    return false;
  }

  This->lastUpdateTime = now;

  double renderInterval =
      static_cast<double>(now - This->lastRenderTime) / CLOCKS_PER_SEC;
  if (renderInterval >= 1.0 / DataManager::GetInstance().system.TargetFPS) {
    DataManager::GetInstance().system.CurrentFPS =
        1.0 / renderInterval;  // 초당 프레임 수
    This->renderManager.Render();
    This->lastRenderTime = now;
  }

  return true;
}
