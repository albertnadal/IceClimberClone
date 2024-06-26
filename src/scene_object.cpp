#include "scene_object.h"
#include <collision/collision.h>
#include <MersenneTwister/MersenneTwister.h>

ISceneObject::ISceneObject() {
  id = SceneObjectIdentificator::NONE;
  MersenneTwister rng;
  uniqueId = rng.integer(0, UINT_MAX);
  boundingBox = solidBoundingBox = {0, 0, 0, 0};
  recalculateAreasDataIsNeeded = true;
  isBreakable = false;
  isMarkedToDelete = false;
}

ISceneObject::ISceneObject(SceneObjectIdentificator _id, SceneObjectType _type, unsigned char _maxStates, bool _isBreakable) :
  StateMachine(_maxStates),
  id(_id),
  type(_type),
  isBreakable(_isBreakable) {
  MersenneTwister rng;
  uniqueId = rng.integer(0, UINT_MAX);
  boundingBox = solidBoundingBox = {0, 0, 0, 0};
  recalculateAreasDataIsNeeded = true;
}

void ISceneObject::SetSpacePartitionObjectsTree(aabb::Tree<ISceneObject*> *_spacePartitionObjectsTree) {
  spacePartitionObjectsTree = _spacePartitionObjectsTree;
}

/*
std::vector<Area>& ISceneObject::GetSolidAreas() {
  if(recalculateAreasDataIsNeeded) {
    // Updates the object solid areas values according to the current object position and current sprite areas
    solidAreas.clear();
    if(currentSprite.areas!=nullptr) {
      for(uint16_t i=0; i<currentSprite.areas->solidAreas.size(); i++) {
        std::vector<collision::vec2<float>> updatedPoints;
        std::vector<collision::vec2<float>> &currentPoints = currentSprite.areas->solidAreas.at(i).rectangle.vertices;
        for(uint16_t j=0; j<currentPoints.size(); j++) {
          // Apply the current position to the current area point
          updatedPoints.push_back(collision::vec2<float>(currentPoints.at(j).x + position.GetX(), currentPoints.at(j).y + position.GetY()));
        }
        collision::Rectangle updatedRectangle(updatedPoints);
        solidAreas.push_back({currentSprite.areas->solidAreas.at(i).id, updatedRectangle});
      }
    }
    recalculateAreasDataIsNeeded = false;
  }
  return solidAreas;
}
*/
/*
std::vector<Area>& ISceneObject::GetSimpleAreas() {

}
*/

void ISceneObject::PositionSetXY(float x, float y) {
    position.setXY(x, y);
    recalculateAreasDataIsNeeded = true;
}

void ISceneObject::PositionSetX(float x) {
  position.setX(x);
  recalculateAreasDataIsNeeded = true;
}

void ISceneObject::PositionSetY(float y) {
  position.setY(y);
  recalculateAreasDataIsNeeded = true;
}

void ISceneObject::PositionAddX(float x) {
  position.addX(x);
  recalculateAreasDataIsNeeded = true;
}

void ISceneObject::PositionAddY(float y) {
  position.addY(y);
  recalculateAreasDataIsNeeded = true;
}

void ISceneObject::PositionSetOffset(int16_t x, int16_t y) {
  position.setOffset(x, y);
  recalculateAreasDataIsNeeded = true;
}

void ISceneObject::RecoverPreviousPosition() {
  position.recoverPreviousPosition();
  recalculateAreasDataIsNeeded = true;
}

void ISceneObject::RemoveFromSpacePartitionObjectsTree() {
  spacePartitionObjectsTree->removeParticle(this);
}

std::vector<uint16_t> ISceneObject::GetLowerBound() {
  std::vector<uint16_t> lowerBound{static_cast<uint16_t>(position.GetIntX() + boundingBox.lowerBoundX), static_cast<uint16_t>(position.GetIntY() + boundingBox.lowerBoundY)};
  return lowerBound;
}

std::vector<uint16_t> ISceneObject::GetUpperBound() {
  std::vector<uint16_t> upperBound{static_cast<uint16_t>(position.GetIntX() + boundingBox.upperBoundX), static_cast<uint16_t>(position.GetIntY() + boundingBox.upperBoundY)};
  return upperBound;
}

std::vector<uint16_t> ISceneObject::GetSolidLowerBound() {
  std::vector<uint16_t> lowerBound{static_cast<uint16_t>(position.GetIntX() + solidBoundingBox.lowerBoundX), static_cast<uint16_t>(position.GetIntY() + solidBoundingBox.lowerBoundY)};
  return lowerBound;
}

std::vector<uint16_t> ISceneObject::GetSolidUpperBound() {
  std::vector<uint16_t> upperBound{static_cast<uint16_t>(position.GetIntX() + solidBoundingBox.upperBoundX), static_cast<uint16_t>(position.GetIntY() + solidBoundingBox.upperBoundY)};
  return upperBound;
}

Boundaries ISceneObject::GetAbsoluteBoundaries() {
  return {static_cast<uint16_t>(position.GetIntX() + boundingBox.upperBoundX),
          static_cast<uint16_t>(position.GetIntY() + boundingBox.upperBoundY),
          static_cast<uint16_t>(position.GetIntX() + boundingBox.lowerBoundX),
          static_cast<uint16_t>(position.GetIntY() + boundingBox.lowerBoundY)};
}

Boundaries ISceneObject::GetAbsoluteSolidBoundaries() {
  return {static_cast<uint16_t>(position.GetIntX() + boundingBox.upperBoundX),
          static_cast<uint16_t>(position.GetIntY() + boundingBox.upperBoundY),
          static_cast<uint16_t>(position.GetIntX() + boundingBox.lowerBoundX),
          static_cast<uint16_t>(position.GetIntY() + boundingBox.lowerBoundY)};
}

SceneObjectIdentificator ISceneObject::Id() {
  return id;
}

SceneObjectType ISceneObject::Type() {
  return type;
}

uint16_t ISceneObject::Width() {
  return currentSprite.width;
}

uint16_t ISceneObject::Height() {
  return currentSprite.height;
}

void ISceneObject::PrintName() {
  std::cout << "PrintName not overloaded for object." << std::endl;
}

void ISceneObject::PrintBoundaries() {
  std::vector<uint16_t> lowerBound = GetLowerBound();
  std::vector<uint16_t> upperBound = GetUpperBound();
  std:cout << "Lowerbound X: " << lowerBound[0] << " Y: " << lowerBound[1] << " | Upperbound X: " << upperBound[0] << " Y: " << upperBound[1] << endl;
}

bool ISceneObject::Update() {
  return Update(KeyboardKeyCode::IC_KEY_NONE);
}

bool ISceneObject::Update(const uint8_t pressedKeys) {
  return false;
}

bool ISceneObject::Update(const uint8_t pressedKeys, aabb::Tree<ISceneObject*>& spacePartitionObjectsTree) {
  return false;
}

void ISceneObject::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
  spriteSheet = _spriteSheet;
}

void ISceneObject::Hit(bool) {
}
