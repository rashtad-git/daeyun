#pragma once

#include <deque>
#include <map>

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
  std::deque<std::pair<double, int>> judgePoints;
  // judgePoint, count
  std::map<int, int> judgeCount;
};
