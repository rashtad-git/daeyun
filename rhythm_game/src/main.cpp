#include <iostream>

#include "System/GameManager.h"

int main() {
  try {
    GameManager gameManager;
    gameManager.run();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
