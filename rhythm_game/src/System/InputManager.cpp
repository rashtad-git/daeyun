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

void InputManager::Init() {}

void InputManager::Update(double deltaTime) {
  UserData& user = DataManager::GetInstance().user;
  for (auto& input : user.input) {
    bool wasPressed = input.second.pressed;
    bool pressed = (GetAsyncKeyState(input.second.key) & 0x8000);

    bool tapped = pressed && !wasPressed;
    bool released = !pressed && wasPressed;

    input.second.tapped = false;
    input.second.pressed = pressed;
    if (tapped) {
      input.second.tapped = true;
      input.second.effect = true;
      input.second.effectFrame = 0;
    }

    if (input.second.effect) {
      input.second.effectFrame += deltaTime;
      if (input.second.effectFrame >= 0.3) {
        input.second.effect = false;
        input.second.effectFrame = -1;
      }
    }
  }
}
