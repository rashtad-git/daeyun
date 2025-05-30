#pragma once

class Node;

class StateManager {
 public:
  StateManager();
  ~StateManager();

 public:
  void Init();
  void Update(double deltaTime);

 private:
  struct PrivateData;
  PrivateData* This;

 private:
  StateManager(const StateManager&) = delete;
  StateManager& operator=(const StateManager&) = delete;
};
