#include "ScoreScreen.h"

#include "../../System/DataManager.h"

const int SCORE_LINE_PERFECT = 0;
const int SCORE_LINE_GREAT = 1;
const int SCORE_LINE_GOOD = 2;
const int SCORE_LINE_BAD = 3;
const int SCORE_LINE_MISS = 4;

const int SCORE_LINE_FAST = 7;
const int SCORE_LINE_SLOW = 8;

int GetScoreHeight(ScoreTypes score) {
  switch (score) {
    case ScoreTypes::Miss:
      return SCORE_LINE_MISS;
    case ScoreTypes::Bad:
      return SCORE_LINE_BAD;
    case ScoreTypes::Good:
      return SCORE_LINE_GOOD;
    case ScoreTypes::Great:
      return SCORE_LINE_GREAT;
    case ScoreTypes::Perfect:
      return SCORE_LINE_PERFECT;
  }

  return -1;
}

ScoreScreen::ScoreScreen() {
  SetPosition(Point(23, 2));

  //// size width
  // max width: 14 = Perfect(7) + '   '(3) + 0000(4)
  // height: 9 = Perfect, Great, Good, Bad, Miss, Space 2, FAST, LATE

  SetSize(Size(14, 9));
}

ScoreScreen::~ScoreScreen() {}

void ScoreScreen::Init() {
  DrawString(0, SCORE_LINE_PERFECT, "PERFECT");
  DrawString(0, SCORE_LINE_GREAT, "GREAT");
  DrawString(0, SCORE_LINE_GOOD, "GOOD");
  DrawString(0, SCORE_LINE_BAD, "BAD");
  DrawString(0, SCORE_LINE_MISS, "MISS");

  DrawString(0, SCORE_LINE_FAST, "FAST");
  DrawString(0, SCORE_LINE_SLOW, "SLOW");

  DrawString(13, SCORE_LINE_PERFECT, "0");
  DrawString(13, SCORE_LINE_GREAT, "0");
  DrawString(13, SCORE_LINE_GOOD, "0");
  DrawString(13, SCORE_LINE_BAD, "0");
  DrawString(13, SCORE_LINE_MISS, "0");
  DrawString(13, SCORE_LINE_FAST, "0");
  DrawString(13, SCORE_LINE_SLOW, "0");
}

void ScoreScreen::PreRender() {

  const auto& user = DataManager::GetInstance().user;
  for (int i = (int)ScoreTypes::Miss; i <= (int)ScoreTypes::Perfect; i++) {
    auto type = (ScoreTypes)i;

    int y = GetScoreHeight(type);

    auto iter = user.scores.find(type);
    int score = 0;
    if (iter != user.scores.end()) {
      score = iter->second;
    }

    DrawNumber(10, y, score, 4);
  }

  DrawNumber(10, SCORE_LINE_FAST, user.fastCount, 4);
  DrawNumber(10, SCORE_LINE_SLOW, user.slowCount, 4);
}

void ScoreScreen::PostRender() {
  for (int i = (int)ScoreTypes::Miss; i <= (int)ScoreTypes::Perfect; i++) {
    // 1�� �ڸ��� ����
    for (int y = 10; y < 13; y++) {
      SetBoard(Point(y, i), ' ');
    }
  }
}
