#include "scene_object.h"

SceneObject::SceneObject() {
  this->position.x = 100.0;
  this->position.y = 100.0;
}

uint16 SceneObject::getWidth() {
  return this->currentSprite.width;
}

uint16 SceneObject::getHeight() {
  return this->currentSprite.height;
}

SceneObject::~SceneObject() {

}
