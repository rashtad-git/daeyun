#include "MetronomeScreen.h"

#include "../../System/DataManager.h"

const int OFFSET_TOP = 2;
const int OFFSET_BOTTOM = 3;

MetronomeScreen::MetronomeScreen() : prevBeatIndex(0) {
  SetPosition(Point(40, 2));
}

MetronomeScreen::~MetronomeScreen() {}

void MetronomeScreen::Init() {
  // SetSize(Size(38,  12));
  auto& game = DataManager::GetInstance().game;
  auto size = Size(38, OFFSET_TOP + OFFSET_BOTTOM + game.BeatInfo.Bottom);
  SetSize(size);

  // guide line
  for (int x = 0; x < size.width; x++) {
    for (int y = 0; y < size.height; y++) {
      Point p(x, y);
      if (x == 0 || x == size.width - 1) {
        SetBoard(p, '|');
      }

      if (y == 0 || y == size.height - 1) {
        SetBoard(p, '-');
      }
    }
  }

  // guide line
  SetBoard(Point(0, 0), '+');
  SetBoard(Point(size.width - 1, 0), '+');
  SetBoard(Point(0, size.height - 1), '+');
  SetBoard(Point(size.width - 1, size.height - 1), '+');
  SetBoard(Point(2, 0), ' ');
  DrawString(3, 0, "Metronome");
  SetBoard(Point(12, 0), ' ');

  for (int i = 0; i < game.BeatInfo.Top; i++) {
    auto p = GetBeatGridPosition(i * game.BeatInfo.Bottom);
    SetBoard(Point(p.x, OFFSET_TOP), 'o');
    SetBoard(Point(p.x, OFFSET_BOTTOM + game.BeatInfo.Bottom), 'o');
  }
}

void MetronomeScreen::PreRender() {
  auto& game = DataManager::GetInstance().game;

  // remove prev
  if (prevBeatIndex != game.currentBeatIndex) {
    auto prevPoint = GetBeatGridPosition(prevBeatIndex);
    SetBoard(prevPoint, ' ');
    SetBoard(Point(prevPoint.x, OFFSET_TOP), 'o');
    SetBoard(Point(prevPoint.x, OFFSET_BOTTOM + game.BeatInfo.Bottom), 'o');
  }

  auto currentPoint = GetBeatGridPosition(game.currentBeatIndex);
  SetBoard(currentPoint, '+');
  SetBoard(Point(currentPoint.x, OFFSET_TOP), 'O');
  SetBoard(Point(currentPoint.x, OFFSET_BOTTOM + game.BeatInfo.Bottom), 'O');

  prevBeatIndex = game.currentBeatIndex;
}

void MetronomeScreen::PostRender() {}

Point MetronomeScreen::GetBeatGridPosition(int beatIndex) const {
  auto& game = DataManager::GetInstance().game;

  int y = beatIndex % game.BeatInfo.Bottom;

  int interval = (36 / game.BeatInfo.Top);
  int offset = interval / 2 - 1;
  int x = 2 + offset + (beatIndex / game.BeatInfo.Bottom) * interval;

  return Point(x, OFFSET_BOTTOM + y);
}
