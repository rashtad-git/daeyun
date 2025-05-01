#pragma once
#include "IRenderer.h"

class ResultRenderer : public IRenderer {
 public:
  virtual void Init() override;
  virtual void PreRender() override;
  virtual char OnRender(const Point& p) override;
  virtual void PostRender() override;

 public:
  ResultRenderer();
  virtual ~ResultRenderer() override;

 private:
  struct PrivateData;
  PrivateData* This;
};
