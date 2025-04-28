#include "StateManager.h"
#include <vector>

#include "DataManager.h"

#include "../Controller/NodeController.h"
#include "../Controller/MeterController.h"

struct StateManager::PrivateData
{
	std::vector<IController*> controllers;
};

StateManager::StateManager()
{
    This = new PrivateData();
	This->controllers.push_back(new NodeController());
	This->controllers.push_back(new MeterController());
}

StateManager::~StateManager()
{
	for (auto& controller : This->controllers)
	{
		delete controller;
	}
    delete This;
}

void StateManager::Init()
{
	for (auto& controller : This->controllers)
	{
		controller->OnInit();
	}
}

void StateManager::Update(double deltaTime)
{
	for (auto controller : This->controllers)
	{
		controller->OnUpdate(deltaTime);
	}
}
