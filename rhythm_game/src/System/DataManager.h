#pragma once

#include "../Utils/Singleton.h"
#include "../Models/SystemData.h"
#include "../Models/UserData.h"
#include "../Models/GameData.h"

class DataManager : public Singleton<DataManager>
{
public:
	SystemData system;
	GameData game;
	UserData user;
};
