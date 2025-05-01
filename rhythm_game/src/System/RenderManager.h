#pragma once

#include <sstream>

class IRenderer;

class RenderManager {
 public:
  RenderManager();
  ~RenderManager();

 public:
  void Init();
  void Render() const;

 private:
  std::stringstream OnRender(IRenderer* renderer) const;

 private:
  void DebugPrint(std::stringstream& buffer, int y) const;

 private:
  struct PrivateData;
  PrivateData* This;

 private:
  RenderManager(const RenderManager&) = delete;
  RenderManager& operator=(const RenderManager&) = delete;
};
