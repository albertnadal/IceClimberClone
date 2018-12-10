#include "scene_object.h"

uint16 ISceneObject::getWidth() {
  return this->currentSprite.width;
}

uint16 ISceneObject::getHeight() {
  return this->currentSprite.height;
}

void ISceneObject::PrintName(){
  std::cout << "PrintName not overloaded for object." << std::endl;
}

void ISceneObject::Update() {
}
