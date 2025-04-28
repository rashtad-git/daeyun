#pragma once

#include "IRenderer.h"

class GameRenderer : public IRenderer {
 public:
  virtual void PreRender() override;
  virtual char OnRender(const Point& p) override;
  virtual void PostRender() override;

 private:
  void OnInitGameScreen();

 public:
  GameRenderer();
  virtual ~GameRenderer() override;

 private:
  struct PrivateData;
  PrivateData* This;
};
