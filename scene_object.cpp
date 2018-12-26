#include "scene_object.h"

SceneObjectIdentificator ISceneObject::Id() {
  return id_;
}

uint16 ISceneObject::GetWidth() {
  return this->currentSprite.width;
}

uint16 ISceneObject::GetHeight() {
  return this->currentSprite.height;
}

void ISceneObject::PrintName(){
  std::cout << "PrintName not overloaded for object." << std::endl;
}

void ISceneObject::Update() {
}

void ISceneObject::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
  spriteSheet = _spriteSheet;
}
