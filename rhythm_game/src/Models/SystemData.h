#pragma once

struct SystemData {
 public:
  bool isPlaying = true;
  double TargetFPS = 60;
  double CurrentFPS;
  double UpdateFrame;
};
