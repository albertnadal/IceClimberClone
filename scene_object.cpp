#include "scene_object.h"

SceneObjectIdentificator ISceneObject::Id() {
  return id_;
}

uint16 ISceneObject::Width() {
  return this->currentSprite.width;
}

uint16 ISceneObject::Height() {
  return this->currentSprite.height;
}

void ISceneObject::PrintName(){
  std::cout << "PrintName not overloaded for object." << std::endl;
}

bool ISceneObject::Update(uchar pressedKeys) {
  return false;
}

void ISceneObject::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
  spriteSheet = _spriteSheet;
}
