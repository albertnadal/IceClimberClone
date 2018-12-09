#include "scene_object_base.h"

SceneObjectBase::SceneObjectBase() {
  this->position.x = 100.0;
  this->position.y = 100.0;
}

uint16 SceneObjectBase::getWidth() {
  return this->currentSprite.width;
}

uint16 SceneObjectBase::getHeight() {
  return this->currentSprite.height;
}

void SceneObjectBase::PrintName(){
  std::cout << "PrintName not overloaded for object." << std::endl;
}

SceneObjectBase::~SceneObjectBase() {

}
