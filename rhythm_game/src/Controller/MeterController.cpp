#include "MeterController.h"

#include "../System/DataManager.h"
#include "../System/SoundManager.h"

MeterController::MeterController() {}

MeterController::~MeterController() {}

void MeterController::OnInit() {
  auto& game = DataManager::GetInstance().game;
  game.currentBeatIndex = game.BeatInfo.GetTotalBeats() - 1;
}

void MeterController::OnUpdate(double deltaTime) {
  auto& game = DataManager::GetInstance().game;
  game.currentBeatFrame += deltaTime;

  double beatInterval = game.BeatInfo.GetBeatInterval();

  if (game.currentBeatFrame > beatInterval) {
    game.currentBeatFrame -= beatInterval;
    game.currentBeatIndex++;
    if (game.currentBeatIndex == game.BeatInfo.GetTotalBeats()) {
      game.currentBeatIndex = 0;
    }

    if (game.IsPlaySound && game.currentBeatIndex % game.BeatInfo.Bottom == 0) {
      if (game.currentBeatIndex == 0)
        SoundManager::GetInstance().Play(880, 150);
      else
        SoundManager::GetInstance().Play(660, 100);
    }
  }
}
