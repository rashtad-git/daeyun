#pragma once

#include "../ScreenBase.h"

class ScoreScreen : public ScreenBase {
 public:
  ScoreScreen();
  virtual ~ScoreScreen() override;

  virtual void Init() override;
  virtual void PreRender() override;
  virtual void PostRender() override;

 private:
  int lastNodeCount;
  int lastMiss;
};
