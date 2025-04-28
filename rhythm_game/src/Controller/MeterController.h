#pragma once

#include "IController.h"

class MeterController : public IController {
 public:
  MeterController();
  virtual ~MeterController() override;

  virtual void OnInit() override;
  virtual void OnUpdate(double deltaTime) override;
};
