#pragma once

class GameManager {
 public:
  GameManager();
  ~GameManager();

 public:
  void run();

 private:
  bool onGameLoop();

 private:
  struct PrivateData;
  PrivateData* This;
};
