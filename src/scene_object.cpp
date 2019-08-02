#include "scene_object.h"
#include <MersenneTwister/MersenneTwister.h>

ISceneObject::ISceneObject() {
  id = SceneObjectIdentificator::NONE;
  MersenneTwister rng;
  uniqueId = rng.integer(0, UINT_MAX);
  boundingBox = {0, 0, 0, 0};
}

ISceneObject::ISceneObject(SceneObjectIdentificator _id, SceneObjectType _type, unsigned char _maxStates) :
  StateMachine(_maxStates),
  id(_id),
  type(_type) {
  MersenneTwister rng;
  uniqueId = rng.integer(0, UINT_MAX);
  boundingBox = {0, 0, 0, 0};
}

SceneObjectIdentificator ISceneObject::Id() {
  return id;
}

SceneObjectType ISceneObject::Type() {
  return type;
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
