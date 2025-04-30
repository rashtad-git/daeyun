#include "StateManager.h"
#include <vector>

#include "DataManager.h"

#include "../Controller/MeterController.h"
#include "../Controller/NodeController.h"

struct StateManager::PrivateData {
  std::vector<IController*> controllers;
};

StateManager::StateManager() {
  This = new PrivateData();
  This->controllers.push_back(new NodeController());
  This->controllers.push_back(new MeterController());
}

StateManager::~StateManager() {
  for (auto& controller : This->controllers) {
    delete controller;
  }
  delete This;
}

void StateManager::Init() {
  auto& game = DataManager::GetInstance().game;

  game.GameState = GameState::Game;
  game.GameTime = 0;
  // game.IsPlaySound = true;
  game.Difficulty = Difficulty::Expert;
  game.NoteSpeed = 2;
  game.TimeSig.BPM = 160;
  game.TimeSig.Top = 4;
  game.TimeSig.Bottom = 4;
  game.TimeSigIndex = 0;
  game.TimeSigFrame = 0;

  game.StageNodeCount = 0;
  game.StageNodes.clear();

  double mul = 1.5;
  game.Judge_Perfect = 0.04 * mul;
  game.Judge_Great = 0.09 * mul;
  game.Judge_Good = 0.15 * mul;
  game.Judge_Bad = 0.3 * mul;

  for (auto& controller : This->controllers) {
    controller->OnInit();
  }
}

void StateManager::Update(double deltaTime) {
  auto& game = DataManager::GetInstance().game;
  game.GameTime += deltaTime;

  for (auto controller : This->controllers) {
    controller->OnUpdate(deltaTime);
  }
}
