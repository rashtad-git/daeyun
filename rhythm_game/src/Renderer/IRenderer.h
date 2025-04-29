#pragma once

#include "../Models/Math.h"

class IRenderer {
 public:
  virtual ~IRenderer() = 0;

  virtual void Init() = 0;
  virtual void PreRender() = 0;
  virtual char OnRender(const Point& p) = 0;
  virtual void PostRender() = 0;
};

inline IRenderer::~IRenderer() {}
