#include "InputManager.h"
#include <Windows.h>

#include "DataManager.h"

struct InputManager::PrivateData {};

InputManager::InputManager() {
  This = new PrivateData();
}

InputManager::~InputManager() {
  delete This;
}

void InputManager::Update(double deltaTime) {
  UserData& user = DataManager::GetInstance().user;
  for (int i = 0; i < Config::BUTTON_COUNT; i++) {
    bool wasPressed = user.input[i].pressed;
    bool pressed = (GetAsyncKeyState(user.input[i].key) & 0x8000);

    bool tapped = pressed && !wasPressed;
    bool released = !pressed && wasPressed;

    user.input[i].tapped = false;
    user.input[i].pressed = pressed;
    if (tapped) {
      user.input[i].tapped = true;
      user.input[i].effect = true;
      user.input[i].effectFrame = 0;
    }

    if (user.input[i].effect) {
      user.input[i].effectFrame += deltaTime;
      if (user.input[i].effectFrame >= 0.3) {
        user.input[i].effect = false;
        user.input[i].effectFrame = -1;
      }
    }
  }
}
