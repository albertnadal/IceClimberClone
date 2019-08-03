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

std::vector<uint16> ISceneObject::GetLowerBound() {
  std::vector<uint16> lowerBound{static_cast<uint16>(position.int_x + boundingBox.lowerBoundX), static_cast<uint16>(position.int_y + boundingBox.lowerBoundY)};
  return lowerBound;
}

std::vector<uint16> ISceneObject::GetUpperBound() {
  std::vector<uint16> upperBound{static_cast<uint16>(position.int_x + boundingBox.upperBoundX), static_cast<uint16>(position.int_y + boundingBox.upperBoundY)};
  return upperBound;
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

void ISceneObject::PrintName() {
  std::cout << "PrintName not overloaded for object." << std::endl;
}

void ISceneObject::PrintBoundaries() {
  std::vector<uint16> lowerBound = GetLowerBound();
  std::vector<uint16> upperBound = GetUpperBound();
  std:cout << "Lowerbound X: " << lowerBound[0] << " Y: " << lowerBound[1] << " | Upperbound X: " << upperBound[0] << " Y: " << upperBound[1] << endl;
}

bool ISceneObject::Update() {
  return Update(KeyboardKeyCode::KEY_NONE);
}

bool ISceneObject::Update(const uchar pressedKeys) {
  return false;
}

bool ISceneObject::Update(const uchar pressedKeys, aabb::Tree<ISceneObject*>& spacePartitionObjectsTree) {
  return false;
}

void ISceneObject::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
  spriteSheet = _spriteSheet;
}
