#pragma once

#include "../ScreenBase.h"

class JudgeScreen : public ScreenBase {
 public:
  JudgeScreen();
  ~JudgeScreen();

  void Init() override;
  void PreRender() override;
  void PostRender() override;
};
