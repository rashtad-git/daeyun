#pragma once

#include "../ScreenBase.h"

class MetronomeScreen : public ScreenBase {
 public:
  MetronomeScreen();
  virtual ~MetronomeScreen() override;

  virtual void Init() override;
  virtual void PreRender() override;
  virtual void PostRender() override;

 private:
  Point GetBeatGridPosition(int beatIndex) const;

 private:
  int prevBeatIndex;
};
