#pragma once

class InputManager {
 public:
  InputManager();
  ~InputManager();

 public:
  void Init();
  void Update(double deltaTime);

 private:
  struct PrivateData;
  PrivateData* This;

 private:
  InputManager(const InputManager&) = delete;
  InputManager& operator=(const InputManager&) = delete;
};
