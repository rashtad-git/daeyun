#include "Node.h"

Node::Node() {
  Clear();
}

void Node::OnEffect(double deltaTime) {
  OnHit(deltaTime);
}

void Node::Init(int line) {
  Clear();
  this->line = line;
  isActive = true;
  graphic = '-';
}

void Node::Clear() {
  line = 0;
  index = 0;
  isActive = false;
  isHit = false;
  hitFrame = 0;
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
