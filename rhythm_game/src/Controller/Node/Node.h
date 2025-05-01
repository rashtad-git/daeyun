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
  void Back() { index--; }

 public:
  void Init(int line);
  void OnEffect(double deltaTime);
  void Clear();

  double GetElapsed() const;
  bool Move();
  double GetTimeToJudgmentLine() const;

 private:
  void OnHit(double deltaTime);
  void OnMiss(double deltaTime);

 private:
  int line;
  int index;
  bool isMoved;
  bool isHit;
  bool isMiss;
  bool isActive;

  double spawnTime;

  double hitFrame;
  double missFrame;
  char graphic;
};
