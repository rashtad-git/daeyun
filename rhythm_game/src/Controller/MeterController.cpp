#include "MeterController.h"

#include "../System/DataManager.h"
#include "../System/SoundManager.h"

MeterController::MeterController() {}

MeterController::~MeterController() {}

void MeterController::OnInit() {
  auto& game = DataManager::GetInstance().game;
  game.TimeSigIndex = game.TimeSig.GetMaxIndex() - 1;
}

void MeterController::OnUpdate(double deltaTime) {
  auto& game = DataManager::GetInstance().game;
  game.TimeSigFrame += deltaTime;

  double tick = game.TimeSig.GetTick();

  if (game.TimeSigFrame > tick) {
    game.TimeSigFrame -= tick;
    game.TimeSigIndex++;
    if (game.TimeSigIndex == game.TimeSig.GetMaxIndex()) {
      game.TimeSigIndex = 0;
    }

    if (game.IsPlaySound && game.TimeSigIndex % game.TimeSig.Bottom == 0) {
      if (game.TimeSigIndex == 0)
        SoundManager::GetInstance().Play(880, 150);
      else
        SoundManager::GetInstance().Play(660, 100);
    }
  }
}
