#include "SceneController.h"

#include "../System/DataManager.h"
#include "./Node/Node.h"

SceneController::SceneController() {}

SceneController::~SceneController() {}

void SceneController::OnInit() {}

void SceneController::OnUpdate(double deltaTime) {
  auto& data = DataManager::GetInstance();
  auto& game = data.game;

  switch (game.GameState) {
    case GameState::Title:
      OnTitle();
      break;
    case GameState::Result:
      OnResult();
      break;
    case GameState::Game:
      OnGame();
      break;
  }
}

void SceneController::OnTitle() {
  auto& data = DataManager::GetInstance();

  // OptionScene이 하드코딩으로 되어있기 때문에 optionFocus를 같이 맞춰야 한다.
  const ButtonState* input_up = data.user.GetInput(InputControl::Up);
  if (input_up != nullptr && input_up->tapped && data.user.optionFocus > 0) {
    data.user.optionFocus--;
  }

  const ButtonState* input_down = data.user.GetInput(InputControl::Down);
  if (input_down != nullptr && input_down->tapped &&
      data.user.optionFocus < 6) {
    data.user.optionFocus++;
  }

  const ButtonState* input_left = data.user.GetInput(InputControl::Left);
  const ButtonState* input_right = data.user.GetInput(InputControl::Right);
  const ButtonState* input_enter = data.user.GetInput(InputControl::Enter);

  // 세부 설정
  bool nodeChanged = false;

  // 난이도 설정
  if (data.user.optionFocus == 0) {
    if (input_left != nullptr && input_left->tapped) {
      if (data.game.Difficulty != Difficulty::Beginner) {
        data.game.Difficulty = (Difficulty)((int)data.game.Difficulty - 1);
        nodeChanged = true;
      }
    }

    if (input_right != nullptr && input_right->tapped) {
      if (data.game.Difficulty != Difficulty::Master) {
        data.game.Difficulty = (Difficulty)((int)data.game.Difficulty + 1);
        nodeChanged = true;
      }
    }
  }

  // BPM 설정
  if (data.user.optionFocus == 1) {
    if (input_left != nullptr && input_left->tapped) {
      data.game.BeatInfo.BPM = data.game.BeatInfo.BPM - 15;
      if (data.game.BeatInfo.BPM < 15) {
        data.game.BeatInfo.BPM = 15;
      }
      nodeChanged = true;
    }

    if (input_right != nullptr && input_right->tapped) {
      data.game.BeatInfo.BPM = data.game.BeatInfo.BPM + 15;
      if (data.game.BeatInfo.BPM > 300) {
        data.game.BeatInfo.BPM = 300;
      }
      nodeChanged = true;
    }
  }

  // speed 설정
  if (data.user.optionFocus == 2) {
    if (input_left != nullptr && input_left->tapped) {
      if (data.game.NoteSpeed == 2) {
        data.game.NoteSpeed = 1;
        nodeChanged = true;
      } else if (data.game.NoteSpeed == 4) {
        data.game.NoteSpeed = 2;
        nodeChanged = true;
      }
    }

    if (input_right != nullptr && input_right->tapped) {
      if (data.game.NoteSpeed == 1) {
        data.game.NoteSpeed = 2;
        nodeChanged = true;
      } else if (data.game.NoteSpeed == 2) {
        data.game.NoteSpeed = 4;
        nodeChanged = true;
      }
    }
  }

  // Judge Scale 설정
  if (data.user.optionFocus == 3) {
    if (input_left != nullptr && input_left->tapped) {
      if (data.game.JudgeScale > 0.6) {
        data.game.JudgeScale -= 0.1;
      }
    }

    if (input_right != nullptr && input_right->tapped) {
      if (data.game.JudgeScale < 4.9) {
        data.game.JudgeScale += 0.1;
      }
    }
  }

  // 사운드 설정
  if (data.user.optionFocus == 4) {
    if ((input_left != nullptr && input_left->tapped) ||
        (input_right != nullptr && input_right->tapped)) {
      data.game.IsPlaySound = !data.game.IsPlaySound;
    }
  }

  // 디버그 출력
  if (data.user.optionFocus == 5) {
    if ((input_left != nullptr && input_left->tapped) ||
        (input_right != nullptr && input_right->tapped)) {
      data.system.showDebug = !data.system.showDebug;
    }
  }

  // 시작 버튼
  if (data.user.optionFocus == 6) {
    if (input_enter != nullptr && input_enter->tapped) {
      data.game.GameState = GameState::Game;

      data.user.Clear();

      nodeChanged = true;
    }
  }

  if (nodeChanged) {
    for (auto& node : data.game.StageNodes) {
      node->SetActive(false);
    }

    data.game.ClearDefault();
  }
}

void SceneController::OnGame() {
  auto& data = DataManager::GetInstance();

  auto escapeIter = data.user.input.find(InputControl::Escape);
  if (escapeIter != data.user.input.end() && escapeIter->second.tapped) {
    data.game.GameState = GameState::Result;
    data.game.IsPlaySound = false;
    for (auto& node : data.game.StageNodes) {
      node->SetActive(false);
    }
  }
}

void SceneController::OnResult() {
  auto& data = DataManager::GetInstance();

  auto escapeIter = data.user.input.find(InputControl::Escape);
  if (escapeIter != data.user.input.end() && escapeIter->second.tapped) {
    data.game.GameState = GameState::Title;
    data.game.IsPlaySound = true;

    data.user.Clear();
    data.game.ClearDefault();
  }
}
