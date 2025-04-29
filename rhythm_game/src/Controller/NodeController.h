#pragma once

#include <list>
#include <set>

#include "IController.h"

class Node;

class NodeController : public IController {
 public:
  NodeController();
  virtual ~NodeController() override;

  virtual void OnInit() override;
  virtual void OnUpdate(double deltaTime) override;

 private:
  void OnSpawn(double deltaTime);
  void OnMove(double deltaTime);
  void OnJudge(double deltaTime);
  void OnCleanup(double deltaTime);

 private:
  void InitJudge();
  void GenNode(int line);
  bool JudgeNode(Node* node) const;

 private:
  double GetDuration(Node* node) const;

 private:
  int prevSigIndex;
  int prevTickIndex;
  bool movedFrame;
  int spawnCount;
  std::set<int> prevLines;
  std::list<Node*> nodePool;
  int startIndex;
};
