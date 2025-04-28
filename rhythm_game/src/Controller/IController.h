#pragma once


class IController
{
public:
	virtual ~IController() = 0;

	virtual void OnInit() = 0;
	virtual void OnUpdate(double deltaTime) = 0;
};

inline IController::~IController() {}
