#pragma once

#include "../Models/GameData.h"
#include "../Models/SystemData.h"
#include "../Models/UserData.h"
#include "../Utils/Singleton.h"

class DataManager : public Singleton<DataManager> {
 public:
  SystemData system;
  GameData game;
  UserData user;
};
