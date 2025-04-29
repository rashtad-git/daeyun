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

  InitJudge();
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

  int count = 1;
  switch (game.Difficulty) {
      // Beginner�� ù�ڸ� 1���� ��Ʈ ����
    case Difficulty::Beginner:
      //if (sigIndex == 0)
      if (sigIndex % game.TimeSig.Bottom == 0)
        break;
      return;

      // easy�� ù�ڿ� �ִ� 2���� ��Ʈ�� �����ǰ� �ڸ��� ���� ��Ʈ�� ����
    case Difficulty::Easy:
      if (sigIndex == 0) {
        count = Math::GetRandom(0, 1) + 1;
        break;
      } else if (sigIndex % game.TimeSig.Bottom == 0) {
        if (Math::GetRandom(0, game.TimeSig.Bottom) == 0)
          break;
      }
      return;
      // easy �� ������ �� ���� �󵵷� ��Ʈ ����, �ڿ��� 2���� ��Ʈ ����
    case Difficulty::Normal:
      count = Math::GetRandom(0, 1) + 1;
      if (sigIndex == 0) {
        break;
      } else if (sigIndex % game.TimeSig.Bottom == 0) {
        if (Math::GetRandom(0, 1) == 0)
          break;
      }
      return;

      // �� �ڿ� �ִ� 2���� ��Ʈ ����, 8����ǥ ����
    case Difficulty::Hard:
      if (sigIndex == 0) {
        count = Math::GetRandom(0, 1) + 1;
        break;
      } else if (sigIndex % game.TimeSig.Bottom == 0) {
        count = Math::GetRandom(0, 1) + 1;
        if (Math::GetRandom(0, 1) == 0)
          break;
      } else if (sigIndex % (game.TimeSig.Bottom / 2) == 0) {
        if (Math::GetRandom(0, game.TimeSig.Bottom) == 0)
          break;
      }
      return;

      // ù�ڿ� �ִ� 3�� ��Ʈ ����,
    case Difficulty::VeryHard:
      if (sigIndex == 0) {
        count = Math::GetRandom(0, 2) + 1;
        break;
      } else if (sigIndex % (game.TimeSig.Bottom / 2) == 0) {
        count = Math::GetRandom(0, 1) + 1;
        if (Math::GetRandom(0, 3) != 0)
          break;
      }
      return;

      // 16�� ��ǥ ����
    case Difficulty::Expert:
      if (sigIndex == 0) {
        count = Math::GetRandom(0, 2) + 1;
        break;
      } else if (sigIndex % game.TimeSig.Bottom == 0) {
        count = Math::GetRandom(0, 1) + 1;
        break;
      } else if (sigIndex % (game.TimeSig.Bottom / 4) == 0) {
        if (Math::GetRandom(0, 1) == 0)
          break;
      }
      return;

    case Difficulty::Master:
      if (sigIndex % game.TimeSig.Bottom == 0) {
        count = 2;
      }
      break;
  }

  std::set<int> lines(prevLines);
  prevLines.clear();
  for (int i = 0; i < count; i++) {
    int line = Math::GetRandom(0, 3, lines);
    GenNode(line);

    prevLines.insert(line);
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

  for (int line = 0; line < Config::BUTTON_COUNT; line++) {
    const auto& button = user.input.find(ActionButtons[line]);
    if (button == user.input.end())
      continue;

    if (button->second.tapped == false)
      continue;

    for (auto node : game.StageNodes) {
      if (node->IsActive() == false || node->HasEffect() ||
          node->GetLine() != line)
        continue;

      int index = node->GetIndex();
      if (!JudgeNode(node))
        continue;

      break;
    }
  }

  for (Node* node : game.StageNodes) {
    if (node->IsActive() == false || node->HasEffect())
      continue;

    if (game.Judge_Bad < GetDuration(node)) {
      node->SetMiss();
      user.scores[ScoreTypes::Miss]++;
    } else if (node->GetIndex() > Config::GetJudgeEnd()) {
      node->Back();
      node->SetMiss();
      user.scores[ScoreTypes::Miss]++;
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

  for (Node* node : deactivated) {
    game.StageNodes.remove(node);
    nodePool.push_back(node);
  }
}

void NodeController::InitJudge() {
  auto& game = DataManager::GetInstance().game;

  // // Judge
  // const double tick = game.TimeSig.GetTick();

  // game.Judge_Perfect = tick / 4;
  // game.Judge_Great = tick / 2;
  // game.Judge_Good = tick;
  // game.Judge_Bad = tick * 2;
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

bool NodeController::JudgeNode(Node* node) const {
  auto& game = DataManager::GetInstance().game;
  auto& user = DataManager::GetInstance().user;

  double duration = GetDuration(node);

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
      user.fastIndicator.push_back(judge);
    } else {
      user.lateIndicator.push_back(judge);
    }
  }

  node->SetHit();
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
