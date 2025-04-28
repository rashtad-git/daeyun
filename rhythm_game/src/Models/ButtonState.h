#pragma once

struct ButtonState {
  char key;
  bool pressed;
  bool tapped;

  bool effect;
  double effectFrame;

  ButtonState(char key)
      : key(key),
        pressed(false),
        tapped(false),
        effect(false),
        effectFrame(-1) {}
};
