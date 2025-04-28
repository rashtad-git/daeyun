#pragma once
#include "IRenderer.h"

class TitleRenderer : public IRenderer {
 public:
  virtual void PreRender() override;
  virtual char OnRender(const Point& p) override;
  virtual void PostRender() override;

 public:
  TitleRenderer();
  virtual ~TitleRenderer() override;

 private:
  struct PrivateData;
  PrivateData* This;
};
