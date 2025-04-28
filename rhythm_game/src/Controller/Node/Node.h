#pragma once

class Node {
 public:
  Node();

 public:
  int GetLine() const { return line; }

  void SetIsHit(bool isHit) { this->isHit = isHit; }
  bool IsHit() const { return isHit; }

  bool IsActive() const { return isActive; }
  void SetActive(bool active) { isActive = active; }

  char GetGraphic() const { return graphic; }

  int GetIndex() const { return index; }
  void Move() { index++; }

 public:
  void Init(int line);
  void OnEffect(double deltaTime);
  void Clear();

 private:
  void OnHit(double deltaTime);

 private:
  int line;
  int index;
  bool isHit;
  bool isActive;

  double hitFrame;
  char graphic;
};
