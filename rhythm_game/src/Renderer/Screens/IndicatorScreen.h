#pragma once

#include <map>
#include <queue>

#include "../ScreenBase.h"

class IndicatorScreen : public ScreenBase {
 public:
  IndicatorScreen();
  virtual ~IndicatorScreen() override;

  virtual void Init() override;
  virtual void PreRender() override;
  virtual void PostRender() override;

 private:
  int IndicateToPoint(double indicate);

  // gameTime, judgePoint
  int prevCount;
  std::queue<std::pair<double, int>> judgePoints;
  std::map<int, int> judgeCount;
};
