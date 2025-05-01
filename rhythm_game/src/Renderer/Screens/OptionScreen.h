#pragma once

#include "../ScreenBase.h"

class OptionScreen : public ScreenBase {
 public:
  OptionScreen();
  virtual ~OptionScreen() override;

  virtual void Init() override;
  virtual void PreRender() override;
  virtual void PostRender() override;
};