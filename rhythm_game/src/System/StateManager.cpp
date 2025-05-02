#include "StateManager.h"
#include <vector>

#include "DataManager.h"

#include "../Controller/MeterController.h"
#include "../Controller/NodeController.h"
#include "../Controller/SceneController.h"

struct StateManager::PrivateData {
  std::vector<IController*> controllers;
};

StateManager::StateManager() {
  This = new PrivateData();
  This->controllers.push_back(new NodeController());
  This->controllers.push_back(new MeterController());
  This->controllers.push_back(new SceneController());

  // 초기값 세팅
  auto& data = DataManager::GetInstance();

  data.game.GameState = GameState::Title;
  data.game.GameTime = 0;
  data.game.IsPlaySound = true;
  data.game.Difficulty = Difficulty::Normal;
  // 1, 2, 4
  data.game.NoteSpeed = 2;
  // 노트가 움직이는 도트가 제한되어있어서 15의 배수로 고정. 최대 300까지 제한 둔다.
  data.game.BeatInfo.BPM = 120;
  // 박자는 4/4박자를 유지한다.
  data.game.BeatInfo.Top = 4;
  data.game.BeatInfo.Bottom = 4;
  data.game.currentBeatIndex = 0;
  data.game.currentBeatFrame = 0;

  data.game.StageNodeCount = 0;
  data.game.StageNodes.clear();

  data.game.JudgeScale = 2;
}

StateManager::~StateManager() {
  for (auto& controller : This->controllers) {
    delete controller;
  }
  delete This;
}

void StateManager::Init() {
  auto& data = DataManager::GetInstance();
  data.game.ClearDefault();
  data.user.Clear();

  for (auto& controller : This->controllers) {
    controller->Init();
  }
}

void StateManager::Update(double deltaTime) {
  auto& game = DataManager::GetInstance().game;
  game.GameTime += deltaTime;

  for (auto controller : This->controllers) {
    controller->Update(deltaTime);
  }
}
