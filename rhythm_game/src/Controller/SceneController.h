#pragma once

#include "IController.h"

class SceneController : public IController {
 public:
  SceneController();
  virtual ~SceneController() override;

 private:
  void OnInit() override;
  void OnUpdate(double deltaTime) override;

 private:
  void OnTitle();
  void OnGame();
  void OnResult();
};
