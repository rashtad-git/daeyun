#pragma once

#include "../Models/Math.h"

class ScreenBase {
 public:
  ScreenBase() = default;
  virtual ~ScreenBase() = 0;

  virtual void Init() = 0;
  virtual void PreRender() = 0;
  virtual void PostRender() = 0;

 public:
  bool Contains(const Point& p) const { return screen.Contains(p); }
  const Point& GetPosition() const { return screen.position; }
  void SetPosition(const Point& p) { screen.position = p; }

  const char GetBoard(const Point& p) const;

 protected:
  bool SetBoard(const Point& p, char c);

  void SetSize(const Size& size);
  const Size& GetSize() const { return screen.size; }

 protected:
  void DrawNumber(int x, int y, int number, int space);

  template <int length>
  void DrawString(int x, int y, const char (&message)[length]) {
    DrawString(x, y, message, length);
  }
  void DrawString(int x, int y, const char* message, int length);
  void DrawChar(int x, int y, char c) { SetBoard(Point(x, y), c); }
  void DrawDouble(int x, int y, double value, int precision);

 private:
  int GetIndex(const Point& p) const { return p.y * screen.size.width + p.x; }

 private:
  Rectangle screen;
  char* board = nullptr;

 private:
  // Copy constructor is deleted because the class manages dynamic memory (new/delete).
  ScreenBase(const ScreenBase&) = delete;
  // Copy assignment operator is deleted to prevent shallow copying of dynamically allocated resources.
  ScreenBase& operator=(const ScreenBase&) = delete;
};

inline ScreenBase::~ScreenBase() {
  if (board != nullptr) {
    delete[] board;
  }
}
