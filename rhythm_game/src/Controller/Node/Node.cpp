#include "Node.h"

Node::Node() {
  Clear();
}

void Node::OnEffect(double deltaTime) {
  OnHit(deltaTime);
  OnMiss(deltaTime);
}

void Node::Init(int line, int index) {
  Clear();
  this->line = line;
  this->index = index;
  isActive = true;
  graphic = '-';
}

void Node::Clear() {
  line = 0;
  index = 0;
  isActive = false;
  isHit = false;
  isMiss = false;
  hitFrame = 0;
  missFrame = 0;
  graphic = ' ';
}

void Node::OnHit(double deltaTime) {
  if (isHit == false)
    return;

  // hit이 되었을 경우 제자리에서 폭파 연출
  hitFrame += deltaTime;

  if (hitFrame <= 0.1) {
    graphic = '*';
  } else if (hitFrame <= 0.2) {
    graphic = 'o';
  } else if (hitFrame <= 0.3) {
    graphic = 'O';
  } else {
    // hit이 종료되면 노드는 deactive가 되며 사라진다.
    Clear();
  }
}

void Node::OnMiss(double deltaTime) {
  if (isMiss == false)
    return;

  missFrame += deltaTime;

  if (missFrame <= 0.1) {
    graphic = 'x';
  } else if (missFrame <= 0.2) {
    graphic = '-';
  } else if (missFrame <= 0.3) {
    graphic = ' ';
  } else {
    Clear();
  }
}
