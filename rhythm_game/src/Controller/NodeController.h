#pragma once

#include <list>
#include <set>

#include "IController.h"

class Node;

class NodeController : public IController {
 public:
  NodeController();
  virtual ~NodeController() override;

 protected:
  virtual void OnInit() override;
  virtual void OnUpdate(double deltaTime) override;

 private:
  void OnSpawn(double deltaTime);
  void OnMove(double deltaTime);
  void OnJudge(double deltaTime);
  void OnCleanup(double deltaTime);

 private:
  int GenerateNodeCountPerBeat(int beatIndex) const;
  void GenerateNode(int count);
  bool JudgeNode(double duration) const;

 private:
  int prevBeatIndex;
  std::set<int> spawnLines;
  std::list<Node*> nodePool;
  bool started;
};
