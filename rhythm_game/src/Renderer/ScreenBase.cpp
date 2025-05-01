#include "ScreenBase.h"

#include <cstdio>

const char ScreenBase::GetBoard(const Point& p) const {
  int i = GetIndex(p);
  if (i < 0 || i >= screen.size.width * screen.size.height)
    return ' ';

  return board[i];
}

bool ScreenBase::SetBoard(const Point& p, char c) {
  int i = GetIndex(p);
  if (i < 0 || i >= screen.size.width * screen.size.height)
    return false;

  board[i] = c;
  return true;
}

void ScreenBase::SetSize(const Size& size) {
  if (board != nullptr) {
    delete[] board;
  }

  screen.size = size;
  int length = size.width * size.height;
  board = new char[length];
  for (int i = 0; i < length; i++) {
    board[i] = ' ';
  }
}

void ScreenBase::DrawNumber(int x, int y, int number, int space) {
  const char numbers[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

  for (int i = 0; i < space; i++) {
    DrawChar(x + i, y, ' ');
  }

  if (number == 0) {
    DrawChar(x + space - 1, y, numbers[0]);
    return;
  }

  while (number > 0) {
    DrawChar(x + space - 1, y, numbers[number % 10]);
    number /= 10;
    space--;
  }
}

void ScreenBase::DrawString(int x, int y, const char* message, int length) {
  for (int i = 0; i < length - 1; i++) {
    DrawChar(x + i, y, message[i]);
  }
}

void ScreenBase::DrawDouble(int x, int y, double value, int precision) {
  int r = 1;
  for (int i = 0; i < precision; i++) {
    r *= 10;
  }

  int decimal = (int)(value * r) % r;
  int integer = (int)(value * r) / r;

  DrawNumber(x, y, integer, 1);
  DrawChar(x + 1, y, '.');
  DrawNumber(x + 2, y, decimal, precision);
}
