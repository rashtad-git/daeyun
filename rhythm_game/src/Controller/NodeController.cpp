#include "NodeController.h"

#include <list>

#include "../System/DataManager.h"
#include "../Utils/Math.h"
#include "./Node/Node.h"

NodeController::NodeController()
    : prevSigIndex(0), prevTickIndex(0), movedFrame(false), spawnCount(0) {}

NodeController::~NodeController() {
  for (auto node : nodePool) {
    delete node;
  }
}

void NodeController::OnInit() {
  auto& game = DataManager::GetInstance().game;

  prevSigIndex = 0;
  spawnCount = 0;

  InitJudge();
}

void NodeController::OnUpdate(double deltaTime) {
  auto& game = DataManager::GetInstance().game;

  // ���ο� ��� ����
  OnSpawn(deltaTime);

  OnMove(deltaTime);
  OnJudge(deltaTime);

  // ��尡 hit �Ǿ��� �� ����Ʈ�� pool�� �ǵ����� ó��
  OnCleanup(deltaTime);

  prevSigIndex = game.TimeSigIndex;
}

void NodeController::OnSpawn(double deltaTime) {
  auto& game = DataManager::GetInstance().game;

  if (game.TimeSigIndex == prevSigIndex)
    return;

  int sigIndex = (game.TimeSigIndex + Config::GAME_LINE_HEIGHT - 2) %
                 game.TimeSig.GetMaxIndex();

  int count = 1;
  switch (game.Difficulty) {
      // Beginner�� ù�ڸ� 1���� ��Ʈ ����
    case Difficulty::Beginner:
      if (sigIndex == 0)
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

    lines.insert(line);
    prevLines.insert(line);
  }
}

void NodeController::OnMove(double deltaTime) {
  auto& game = DataManager::GetInstance().game;

  double tick = game.TimeSig.GetTick() / game.NoteSpeed;
  double frame = game.TimeSigFrame;
  int tickIndex = 0;
  while (frame > tick) {
    frame -= tick;
    tickIndex++;
  }

  if (game.TimeSigIndex == prevSigIndex && prevTickIndex == tickIndex)
    return;

  prevTickIndex = tickIndex;

  for (auto node : game.StageNodes) {
    if (node->IsActive() == false)
      continue;

    if (node->IsHit())
      continue;

    node->Move();
  }
}

void NodeController::OnJudge(double deltaTime) {
  auto& game = DataManager::GetInstance().game;
  auto& user = DataManager::GetInstance().user;

  for (int line = 0; line < Config::BUTTON_COUNT; line++) {
    if (user.input[line].tapped == false)
      continue;

    for (auto node : game.StageNodes) {
      if (node->IsHit() || node->GetLine() != line)
        continue;

      int index = node->GetIndex();
      if (!JudgeNode(node))
        continue;

      break;
    }
  }

  for (Node* node : game.StageNodes) {
    if (node->GetIndex() > Config::GAME_LINE_HEIGHT) {
      node->SetActive(false);
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

  // Judge
  int perfectJudge = Config::GAME_LINE_HEIGHT - 1;
  const double tick = game.TimeSig.GetTick();

  judgePerfect = tick / 4;
  judgeGreat = tick / 2;
  judgeGood = tick;
  judgeBad = tick * 2;

  //
  judgePerfect = 0.022;
  judgeGreat = 0.045;
  judgeGood = 0.09;
  judgeBad = 0.15;
}

void NodeController::GenNode(int line) {
  auto& game = DataManager::GetInstance().game;

  if (nodePool.size() <= 0) {
    nodePool.push_back(new Node());
  }

  auto node = nodePool.front();
  nodePool.pop_front();

  node->Init(line);
  game.StageNodes.push_back(node);
}

bool NodeController::JudgeNode(Node* node) const {
  auto& game = DataManager::GetInstance().game;
  auto& user = DataManager::GetInstance().user;

  int index = node->GetIndex();
  //if (index < Config::GAME_LINE_HEIGHT - 3)
  //	return false;

  // Judge
  int perfectJudge = Config::GAME_LINE_HEIGHT - 2;
  int judgeLine = index - perfectJudge;
  const double tick = game.TimeSig.GetTick();

  double duration = 0;
  bool isFast = true;
  if (judgeLine >= 0) {
    duration = game.TimeSigFrame + judgeLine * tick;
  } else {
    duration = (tick - game.TimeSigFrame + std::abs(judgeLine + 1) * tick);
  }

  if (duration < judgePerfect) {
    user.scores[ScoreTypes::Perfect]++;

    // perfect�� �� ��ĭ ����
    if (judgeLine < 0)
      node->Move();
  } else if (duration < judgeGreat) {
    user.scores[ScoreTypes::Great]++;
  } else if (duration < judgeGood) {
    user.scores[ScoreTypes::Good]++;
  } else if (duration < judgeBad) {
    user.scores[ScoreTypes::Bad]++;
  } else {
    return false;
  }

  if (duration > judgePerfect) {
    if (judgeLine < 0) {
      user.fastIndicator.push_back(duration);
    } else {
      user.lateIndicator.push_back(duration);
    }
  }

  game.debugJudge += duration;

  node->SetIsHit(true);
  return true;
}
