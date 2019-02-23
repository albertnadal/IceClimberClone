#include "scene_object.h"

ISceneObject::ISceneObject() {
  id = SceneObjectIdentificator::NONE;
}

ISceneObject::ISceneObject(SceneObjectIdentificator _id, unsigned char _maxStates) :
  StateMachine(_maxStates),
  id(_id) {

}

SceneObjectIdentificator ISceneObject::Id() {
  return id;
}

uint16 ISceneObject::Width() {
  return currentSprite.width;
}

uint16 ISceneObject::Height() {
  return currentSprite.height;
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
