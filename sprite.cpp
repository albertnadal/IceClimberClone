#include "sprite.h"

Sprite::Sprite() {
  this->width = 64;
  this->height = 64;
  this->u1 = 0.0f;
  this->v1 = 0.0f;
  this->u2 = 0.5f;
  this->v2 = 0.5f;
  loadTexture();
}

void Sprite::loadTexture() {
}

Sprite::~Sprite() {

}
