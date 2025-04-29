#pragma once

class Node {
 public:
  Node();

 public:
  int GetLine() const { return line; }

  void SetHit() { this->isHit = true; }
  void SetMiss() { this->isMiss = true; }
  bool HasEffect() const { return isHit || isMiss; }

  bool IsActive() const { return isActive; }
  void SetActive(bool active) { isActive = active; }

  char GetGraphic() const { return graphic; }

  int GetIndex() const { return index; }
  void Move() { index++; }
  void Back() { index--; }

 public:
  void Init(int line, int index = 0);
  void OnEffect(double deltaTime);
  void Clear();

 private:
  void OnHit(double deltaTime);
  void OnMiss(double deltaTime);

 private:
  int line;
  int index;
  bool isHit;
  bool isMiss;
  bool isActive;

  double hitFrame;
  double missFrame;
  char graphic;
};
