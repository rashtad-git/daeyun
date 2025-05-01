#include "NodeController.h"

#include <cmath>
#include <list>

#include "../System/DataManager.h"
#include "../Utils/Math.h"
#include "./Node/Node.h"

NodeController::NodeController()
    : prevSigIndex(0),
      prevTickIndex(0),
      movedFrame(false),
      spawnCount(0),
      startIndex(0) {}

NodeController::~NodeController() {
  for (auto node : nodePool) {
    delete node;
  }
}

void NodeController::OnInit() {
  auto& game = DataManager::GetInstance().game;

  prevSigIndex = 0;
  spawnCount = 0;

  int judgeLine = Config::GetPerfectJudge();
  startIndex = -1;  // 1 - judgeLine * game.NoteSpeed;
}

void NodeController::OnUpdate(double deltaTime) {
  auto& game = DataManager::GetInstance().game;

  OnSpawn(deltaTime);

  OnJudge(deltaTime);
  OnMove(deltaTime);

  OnCleanup(deltaTime);

  prevSigIndex = game.TimeSigIndex;
}

void NodeController::OnSpawn(double deltaTime) {
  auto& game = DataManager::GetInstance().game;

  if (game.TimeSigIndex == prevSigIndex)
    return;

  int sigIndex = game.TimeSigIndex;

  int count = 0;
  switch (game.Difficulty) {
    case Difficulty::Beginner:
      if (sigIndex == 0) {
        // 1마디에 1~2개 노트
        count = Math::GetRandom(0, 1) + 1;
      }
      break;

    case Difficulty::Easy:
      if (sigIndex == 0) {
        // 매 마디에 1~2개 노트
        count = Math::GetRandom(0, 1) + 1;
      } else if (sigIndex % game.TimeSig.Bottom == 0) {
        // 매 박자에 1개노트 나올수도 25%
        if (Math::GetRandom(0, 4) == 0) {
          count = 1;
        }
      }
      break;

    case Difficulty::Normal:
      if (sigIndex == 0) {
        // 매 마디에 1~2개 노트 등장
        count = Math::GetRandom(0, 1) + 1;
      } else if (sigIndex % game.TimeSig.Bottom == 0) {
        // 매 박자에 나올수도 50%
        if (Math::GetRandom(0, 1) == 0) {
          // 1~2개 노트
          count = Math::GetRandom(0, 1) + 1;
        }
      }
      break;

    case Difficulty::Hard:
      if (sigIndex == 0) {
        // 매 마디에 1~2개 노트
        count = Math::GetRandom(0, 1) + 1;
      } else if (sigIndex % game.TimeSig.Bottom == 0) {
        // 매 박자에 나올수도 50%
        if (Math::GetRandom(0, 1) == 0) {
          // 1~2개 노트
          count = Math::GetRandom(0, 1) + 1;
        }
      } else if (sigIndex % (game.TimeSig.Bottom / 2) == 0) {
        // 8분음표 나올수도 50%
        if (Math::GetRandom(0, game.TimeSig.Bottom) == 0) {
          count = 1;
        }
      }
      break;

    case Difficulty::VeryHard:
      if (sigIndex == 0) {
        // 매 마디에 1~3개 노트
        count = Math::GetRandom(0, 2) + 1;
      } else if (sigIndex % (game.TimeSig.Bottom / 2) == 0) {
        // 8분음표 나올수도 50%
        if (Math::GetRandom(0, 3) != 0) {
          // 1~2개 노트
          count = Math::GetRandom(0, 1) + 1;
        }
      }
      break;

    case Difficulty::Expert:
      if (sigIndex == 0) {
        // 매 마디에 1~3개 노트
        count = Math::GetRandom(0, 2) + 1;
      } else if (sigIndex % game.TimeSig.Bottom == 0) {
        // 매 박자에 1~2개 노트
        count = Math::GetRandom(0, 1) + 1;
      } else if (sigIndex % (game.TimeSig.Bottom / 4) == 0) {
        // 16분음표 나올수도 50%
        if (Math::GetRandom(0, 1) == 0) {
          count = 1;
        }
      }
      break;

    case Difficulty::Master:
      if (sigIndex == 0) {
        // 매 마디에 0~3개 노트
        count = Math::GetRandom(0, 3);
      } else if (sigIndex % game.TimeSig.Bottom == 0) {
        // 매 박자에 0~2개 노트
        count = Math::GetRandom(0, 2);
      } else if (sigIndex % (game.TimeSig.Bottom / 2) == 0) {
        // 8분음표 0~2개 노트
        count = Math::GetRandom(0, 2);
      } else if (sigIndex % (game.TimeSig.Bottom / 4) == 0) {
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

  if (count <= 0) {
    return;
  }

  std::set<int> lines(spawnLines);
  spawnLines.clear();
  for (int i = 0; i < count; i++) {
    int line = Math::GetRandom(0, 3, lines);
    if (spawnLines.find(line) != spawnLines.end())
      continue;

    GenNode(line);

    spawnLines.insert(line);
  }
}

void NodeController::OnMove(double deltaTime) {
  auto& game = DataManager::GetInstance().game;

  double tick = game.TimeSig.GetTick();
  double frame = game.TimeSigFrame * game.NoteSpeed;
  int tickIndex = static_cast<int>(std::floor(frame / tick));

  if (game.TimeSigIndex == prevSigIndex && prevTickIndex == tickIndex)
    return;

  prevTickIndex = tickIndex;

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

      double duration = GetDuration(node);
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

    double duration = GetDuration(node);
    if (duration > game.Judge_Bad) {
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

void NodeController::GenNode(int line) {
  auto& game = DataManager::GetInstance().game;

  if (nodePool.size() <= 0) {
    nodePool.push_back(new Node());
  }

  auto node = nodePool.front();
  nodePool.pop_front();

  node->Init(line, startIndex);
  game.StageNodes.push_back(node);
}

bool NodeController::JudgeNode(double duration) const {
  auto& game = DataManager::GetInstance().game;
  auto& user = DataManager::GetInstance().user;

  double judge = std::abs(duration);
  if (judge < game.Judge_Perfect) {
    user.scores[ScoreTypes::Perfect]++;
  } else if (judge < game.Judge_Great) {
    user.scores[ScoreTypes::Great]++;
  } else if (judge < game.Judge_Good) {
    user.scores[ScoreTypes::Good]++;
  } else if (judge < game.Judge_Bad) {
    user.scores[ScoreTypes::Bad]++;
  } else {
    return false;
  }

  if (judge >= game.Judge_Perfect) {
    if (duration < 0) {
      user.fastCount++;
    } else {
      user.slowCount++;
    }
  }

  return true;
}

double NodeController::GetDuration(Node* node) const {
  auto& game = DataManager::GetInstance().game;

  int index = node->GetIndex();

  // 판정선. 0이면 판정선 위에 있음
  int judgeLine = index - Config::GetPerfectJudge();

  // 노트 하나 이동하는 시간
  const double tick = game.TimeSig.GetTick() / game.NoteSpeed;

  // 판정선 위에 있는 노트의 시간
  return game.TimeSigFrame / game.NoteSpeed + judgeLine * tick - tick * 0.5;
}
