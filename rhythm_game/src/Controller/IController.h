#pragma once

class IController {
 public:
  virtual ~IController() = 0;

 public:
  void Init() { OnInit(); }
  void Update(double deltaTime) { OnUpdate(deltaTime); }

 protected:
  virtual void OnInit() = 0;
  virtual void OnUpdate(double deltaTime) = 0;
};

inline IController::~IController() {}
