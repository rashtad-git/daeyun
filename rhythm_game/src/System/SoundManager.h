#pragma once

#include "../Utils/Singleton.h"

#include <mutex>
#include <queue>
#include <thread>

class SoundManager : public Singleton<SoundManager> {
 public:
  SoundManager();
  ~SoundManager();

  void Play(int frequency, int duration);

 private:
  void SoundLoop();

 private:
  std::queue<std::pair<int, int>> beepQueue;
  std::mutex beepMutex;
  std::thread soundThread;
  bool running;
};
