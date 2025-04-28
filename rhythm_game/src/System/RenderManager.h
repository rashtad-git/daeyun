#pragma once

#include <sstream>

class IRenderer;

class RenderManager {
 public:
  RenderManager();
  ~RenderManager();

 public:
  void Render() const;

 private:
  std::stringstream OnRender(IRenderer* renderer) const;

 private:
  struct PrivateData;
  PrivateData* This;
};
