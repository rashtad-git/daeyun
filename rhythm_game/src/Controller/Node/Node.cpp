#include "Node.h"

#include <cmath>

#include "../../System/DataManager.h"

Node::Node() {
  Clear();
}

void Node::OnEffect(double deltaTime) {
  OnHit(deltaTime);
  OnMiss(deltaTime);
}

void Node::Init(int line) {
  auto& game = DataManager::GetInstance().game;

  Clear();
  this->line = line;
  index = 0;
  isActive = true;
  graphic = '-';
  spawnTime = game.GameTime;
}

void Node::Clear() {
  line = 0;
  index = 0;
  isMoved = false;
  isActive = false;
  isHit = false;
  isMiss = false;
  hitFrame = 0;
  missFrame = 0;
  graphic = ' ';
  spawnTime = 0;
}

double Node::GetElapsed() const {
  auto& game = DataManager::GetInstance().game;

  return game.GameTime - spawnTime;
}

bool Node::Move() {
  if (IsActive() == false && HasEffect())
    return false;

  auto& game = DataManager::GetInstance().game;

  // 현재 프레임을 한 박자 내로 정규화
  double beatInterval = game.BeatInfo.GetBeatInterval() / game.NoteSpeed;
  double normalizedFrame = fmod(game.currentBeatFrame, beatInterval);

  // 박자의 후반부에만 이동 허용
  if (normalizedFrame < beatInterval * 0.5) {
    isMoved = false;
    return false;
  }

  // 이미 이번 박자에 이동했으면 더 이상 이동하지 않음
  if (isMoved)
    return false;

  isMoved = true;
  index++;

  return true;
}

double Node::GetTimeToJudgmentLine() const {
  auto& game = DataManager::GetInstance().game;

  // 현재 위치에서 판정선까지의 거리 (음수로 계산)
  double distanceToJudgment = index - Config::GetPerfectJudge();

  // 한 박자당 걸리는 시간
  double beatInterval = game.BeatInfo.GetBeatInterval() / game.NoteSpeed;

  // 현재 박자에서의 진행률 (0.0 ~ 1.0)
  double beatProgress =
      fmod(game.currentBeatFrame, beatInterval) / beatInterval;

  // 현재 박자에서 남은 시간
  double remainingBeatTime = beatInterval * (1.0 - beatProgress);

  // index가 증가하는 시점을 고려한 시간 계산
  double totalTime = (distanceToJudgment * beatInterval);

  // 현재 박자의 진행도를 반영
  if (beatProgress >= 0.5) {  // 박자의 후반부
    totalTime -= remainingBeatTime;
  } else {  // 박자의 전반부
    totalTime += beatInterval - remainingBeatTime;
  }

  return totalTime;
}

void Node::OnHit(double deltaTime) {
  if (isHit == false)
    return;

  // hit이 되었을 경우 제자리에서 폭파 연출
  hitFrame += deltaTime;

  if (hitFrame <= 0.1) {
    graphic = '*';
  } else if (hitFrame <= 0.2) {
    graphic = 'o';
  } else if (hitFrame <= 0.4) {
    graphic = 'O';
  } else {
    // hit이 종료되면 노드는 deactive가 되며 사라진다.
    isActive = false;
  }
}

void Node::OnMiss(double deltaTime) {
  if (isMiss == false)
    return;

  missFrame += deltaTime;

  if (missFrame <= 0.1) {
    graphic = 'x';
  } else if (missFrame <= 0.2) {
    graphic = '-';
  } else if (missFrame <= 0.4) {
    graphic = ' ';
  } else {
    isActive = false;
  }
}
