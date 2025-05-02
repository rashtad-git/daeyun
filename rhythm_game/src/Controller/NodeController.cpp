#include "NodeController.h"

#include <list>

#include "../System/DataManager.h"
#include "../Utils/Math.h"
#include "./Node/Node.h"

NodeController::NodeController() : prevBeatIndex(0) {}

NodeController::~NodeController() {
  for (auto node : nodePool) {
    delete node;
  }
}

void NodeController::OnInit() {
  auto& game = DataManager::GetInstance().game;

  prevBeatIndex = 0;
  started = false;
}

void NodeController::OnUpdate(double deltaTime) {
  auto& game = DataManager::GetInstance().game;

  OnCleanup(deltaTime);

  if (game.GameState != GameState::Title && game.GameState != GameState::Game) {
    return;
  }

  OnSpawn(deltaTime);

  OnJudge(deltaTime);
  OnMove(deltaTime);

  prevBeatIndex = game.currentBeatIndex;
}

void NodeController::OnSpawn(double deltaTime) {
  auto& game = DataManager::GetInstance().game;

  if (game.currentBeatIndex == prevBeatIndex)
    return;

  int maxBeat = game.BeatInfo.GetTotalBeats();

  // 판정선까지 도달하는데 걸리는 시간 계산
  double timeToJudgment = static_cast<double>(Config::GetPerfectJudge());

  // 한 비트당 걸리는 시간
  double timePerBeat = game.BeatInfo.GetBeatInterval();

  // 판정선까지 도달하는데 걸리는 비트 수 계산
  int beatsToJudgment =
      static_cast<int>(timeToJudgment / timePerBeat) % maxBeat;

  // 속도 보정
  if (beatsToJudgment == 0) {
    beatsToJudgment = maxBeat - maxBeat / game.NoteSpeed;
  } else {
    beatsToJudgment = maxBeat - (beatsToJudgment / game.NoteSpeed) % maxBeat;
  }

  // 현재 비트에서 판정선까지 도달하는데 걸리는 비트 수를 뺀 인덱스 계산
  int targetBeatIndex =
      (maxBeat + game.currentBeatIndex - beatsToJudgment) % maxBeat;

  // 마디 첫 비트 부터 시작
  if (targetBeatIndex == 0) {
    started = true;
  }

  if (!started) {
    return;
  }

  int count = GenerateNodeCountPerBeat(targetBeatIndex);
  if (count <= 0) {
    return;
  }

  GenerateNode(count);
}

void NodeController::OnMove(double deltaTime) {
  auto& game = DataManager::GetInstance().game;

  for (auto node : game.StageNodes) {
    if (node->IsActive() == false)
      continue;

    if (node->HasEffect())
      continue;

    node->Move();
  }
}

void NodeController::OnJudge(double deltaTime) {
  auto& game = DataManager::GetInstance().game;
  auto& user = DataManager::GetInstance().user;

  // 입력 처리
  for (int line = 0; line < Config::BUTTON_COUNT; line++) {
    const auto& button = user.input.find(ActionButtons[line]);
    if (button == user.input.end())
      continue;

    if (button->second.tapped == false)
      continue;

    for (auto& node : game.StageNodes) {
      if (node->IsActive() == false || node->HasEffect())
        continue;

      if (node->GetLine() != line)
        continue;

      double duration = node->GetTimeToJudgmentLine();
      if (!JudgeNode(duration))
        continue;

      user.indicator.push_back(duration);
      user.comboCount++;
      user.maxComboCount = max(user.maxComboCount, user.comboCount);
      node->SetHit();
      break;
    }
  }

  // miss 판정
  for (auto& node : game.StageNodes) {
    if (node->IsActive() == false || node->HasEffect())
      continue;

    double duration = node->GetTimeToJudgmentLine();
    if (duration > game.GetJudge(ScoreTypes::Bad)) {
      node->SetMiss();
      user.scores[ScoreTypes::Miss]++;
      user.comboCount = 0;
    } else if (node->GetIndex() > Config::GetJudgeEnd()) {
      node->Back();
      node->SetMiss();
      user.scores[ScoreTypes::Miss]++;
      user.comboCount = 0;
    }
  }
}

void NodeController::OnCleanup(double deltaTime) {
  auto& game = DataManager::GetInstance().game;

  std::list<Node*> deactivated;
  for (Node* node : game.StageNodes) {
    node->OnEffect(deltaTime);
    if (node->IsActive() == false) {
      deactivated.push_back(node);
    }
  }

  for (auto& node : deactivated) {
    node->Clear();
    game.StageNodes.remove(node);
    nodePool.push_back(node);
  }
}

int NodeController::GenerateNodeCountPerBeat(int beatIndex) const {
  auto& game = DataManager::GetInstance().game;

  int count = 0;
  switch (game.Difficulty) {
    case Difficulty::Beginner:
      if (beatIndex == 0) {
        // 1마디에 1~2개 노트
        count = Math::GetRandom(0, 1) + 1;
      }
      break;

    case Difficulty::Easy:
      if (beatIndex == 0) {
        // 매 마디에 1~2개 노트
        count = Math::GetRandom(0, 1) + 1;
      } else if (beatIndex % game.BeatInfo.Bottom == 0) {
        // 매 박자에 1개노트 나올수도 25%
        if (Math::GetRandom(0, 4) == 0) {
          count = 1;
        }
      }
      break;

    case Difficulty::Normal:
      if (beatIndex == 0) {
        // 매 마디에 1~2개 노트 등장
        count = Math::GetRandom(0, 1) + 1;
      } else if (beatIndex % game.BeatInfo.Bottom == 0) {
        // 매 박자에 나올수도 50%
        if (Math::GetRandom(0, 1) == 0) {
          // 1~2개 노트
          count = Math::GetRandom(0, 1) + 1;
        }
      }
      break;

    case Difficulty::Hard:
      if (beatIndex == 0) {
        // 매 마디에 1~2개 노트
        count = Math::GetRandom(0, 1) + 1;
      } else if (beatIndex % game.BeatInfo.Bottom == 0) {
        // 매 박자에 나올수도 50%
        if (Math::GetRandom(0, 1) == 0) {
          // 1~2개 노트
          count = Math::GetRandom(0, 1) + 1;
        }
      } else if (beatIndex % (game.BeatInfo.Bottom / 2) == 0) {
        // 8분음표 나올수도 50%
        if (Math::GetRandom(0, game.BeatInfo.Bottom) == 0) {
          count = 1;
        }
      }
      break;

    case Difficulty::VeryHard:
      if (beatIndex == 0) {
        // 매 마디에 1~3개 노트
        count = Math::GetRandom(0, 2) + 1;
      } else if (beatIndex % (game.BeatInfo.Bottom / 2) == 0) {
        // 8분음표 나올수도 50%
        if (Math::GetRandom(0, 3) != 0) {
          // 1~2개 노트
          count = Math::GetRandom(0, 1) + 1;
        }
      }
      break;

    case Difficulty::Expert:
      if (beatIndex == 0) {
        // 매 마디에 1~3개 노트
        count = Math::GetRandom(0, 2) + 1;
      } else if (beatIndex % game.BeatInfo.Bottom == 0) {
        // 매 박자에 1~2개 노트
        count = Math::GetRandom(0, 1) + 1;
      } else if (beatIndex % (game.BeatInfo.Bottom / 4) == 0) {
        // 16분음표 나올수도 50%
        if (Math::GetRandom(0, 1) == 0) {
          count = 1;
        }
      }
      break;

    case Difficulty::Master:
      if (beatIndex == 0) {
        // 매 마디에 0~3개 노트
        count = Math::GetRandom(0, 3);
      } else if (beatIndex % game.BeatInfo.Bottom == 0) {
        // 매 박자에 0~2개 노트
        count = Math::GetRandom(0, 2);
      } else if (beatIndex % (game.BeatInfo.Bottom / 2) == 0) {
        // 8분음표 0~2개 노트
        count = Math::GetRandom(0, 2);
      } else if (beatIndex % (game.BeatInfo.Bottom / 4) == 0) {
        // 16분음표 나올지도
        if (Math::GetRandom(0, 1) == 0) {
          if (Math::GetRandom(0, 5) == 0) {
            // 일반적으론 1개 노트
            count = 1;
          } else {
            // 특수한 경우 2~3개 노트
            count = Math::GetRandom(0, 2) + 1;
          }
        }
      }
      break;
  }

  game.DebugBeatIndex = beatIndex;
  game.DebugSpawnNodeCount += count;

  return count;
}

void NodeController::GenerateNode(int count) {
  auto& game = DataManager::GetInstance().game;

  std::set<int> prevLines(spawnLines);
  spawnLines.clear();
  for (int i = 0; i < count; i++) {
    int line = Math::GetRandom(0, 3, prevLines);
    if (spawnLines.find(line) != spawnLines.end())
      continue;

    if (nodePool.size() <= 0) {
      nodePool.push_back(new Node());
    }

    auto node = nodePool.front();
    nodePool.pop_front();

    node->Init(line);
    game.StageNodes.push_back(node);

    spawnLines.insert(line);
  }
}

bool NodeController::JudgeNode(double duration) const {
  auto& game = DataManager::GetInstance().game;
  auto& user = DataManager::GetInstance().user;

  double judge = std::abs(duration);
  if (judge < game.GetJudge(ScoreTypes::Perfect)) {
    user.scores[ScoreTypes::Perfect]++;
  } else if (judge < game.GetJudge(ScoreTypes::Great)) {
    user.scores[ScoreTypes::Great]++;
  } else if (judge < game.GetJudge(ScoreTypes::Good)) {
    user.scores[ScoreTypes::Good]++;
  } else if (judge < game.GetJudge(ScoreTypes::Bad)) {
    user.scores[ScoreTypes::Bad]++;
  } else {
    return false;
  }

  if (judge >= game.GetJudge(ScoreTypes::Perfect)) {
    if (duration < 0) {
      user.fastCount++;
    } else {
      user.slowCount++;
    }
  }

  return true;
}
