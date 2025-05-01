#pragma once

#include <list>

#include "../Game/Config.h"

class Node;

enum class GameState {
  None,
  Title,
  Game,
  Result,
};

enum class Difficulty {
  None,
  Beginner,
  Easy,
  Normal,
  Hard,
  VeryHard,
  Expert,
  Master,
};

struct TimeSignature {
  int BPM;
  int Top;     // 3, 4, 6, 9, 12
  int Bottom;  // 4, 8

  int GetTotalBeats() const { return Top * Bottom; }
  double GetBarDuration() const { return (60 / (double)BPM) * (double)Top; }
  double GetBeatInterval() const {
    return GetBarDuration() / (double)GetTotalBeats();
  }
};

struct GameData {
  GameState GameState;
  double GameTime;

  bool IsPlaySound = true;
  Difficulty Difficulty;
  int NoteSpeed;
  TimeSignature BeatInfo;
  int currentBeatIndex = 0;  // 0 ~ BeatInfo.Top * BeatInfo.Bottom
  double currentBeatFrame = 0;

  int StageNodeCount;
  std::list<Node*> StageNodes;

  double JudgeScale;

  void ClearDefault() {
    currentBeatIndex = -1;
    currentBeatFrame = 0;
    StageNodeCount = 0;

    DebugBeatIndex = 0;
    DebugNodeCount = 0;
  }

  double GetJudge(ScoreTypes type) const {
    switch (type) {
      case ScoreTypes::Perfect:
        return JudgeScale * StandardJudge_Perfect;
      case ScoreTypes::Great:
        return JudgeScale * StandardJudge_Great;
      case ScoreTypes::Good:
        return JudgeScale * StandardJudge_Good;
      case ScoreTypes::Bad:
        return JudgeScale * StandardJudge_Bad;
      default:
        return 0;
    }
  }

  // debug
  int DebugBeatIndex;
  int DebugNodeCount;
};
