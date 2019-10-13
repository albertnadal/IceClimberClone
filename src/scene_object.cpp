#include "scene_object.h"
#include <collision/collision.h>
#include <MersenneTwister/MersenneTwister.h>

ISceneObject::ISceneObject() {
  id = SceneObjectIdentificator::NONE;
  MersenneTwister rng;
  uniqueId = rng.integer(0, UINT_MAX);
  boundingBox = {0, 0, 0, 0};
  recalculateAreasDataIsNeeded = true;
}

ISceneObject::ISceneObject(SceneObjectIdentificator _id, SceneObjectType _type, unsigned char _maxStates) :
  StateMachine(_maxStates),
  id(_id),
  type(_type) {
  MersenneTwister rng;
  uniqueId = rng.integer(0, UINT_MAX);
  boundingBox = {0, 0, 0, 0};
  recalculateAreasDataIsNeeded = true;
}

void ISceneObject::SetSpacePartitionObjectsTree(aabb::Tree<ISceneObject*> *_spacePartitionObjectsTree) {
  spacePartitionObjectsTree = _spacePartitionObjectsTree;
}

std::vector<Area>& ISceneObject::GetSolidAreas() {
  if(recalculateAreasDataIsNeeded) {
    std::cout << "RECALCULATING SOLID AREAS" << endl;
    // Updates the object solid areas values according to the current object position and current sprite areas
    solidAreas.clear();
    if(currentSprite.areas!=nullptr) {
      std::cout << "CURRENT SPRITE CONTAINS " << currentSprite.areas->solidAreas.size() << " SOLID AREAS" << endl;
      for(uint16_t i=0; i<currentSprite.areas->solidAreas.size(); i++) {
        std::vector<collision::vec2<float>> updatedPoints;
        std::vector<collision::vec2<float>> &currentPoints = currentSprite.areas->solidAreas.at(i).polygon.vertices;
        for(uint16_t j=0; j<currentPoints.size(); j++) {
          // Apply the current position to the current area point
          updatedPoints.push_back(collision::vec2<float>(currentPoints.at(j).x + position.GetX(), currentPoints.at(j).y + position.GetY()));
        }
        collision::Polygon updatedPolygon(updatedPoints);
        solidAreas.push_back({currentSprite.areas->solidAreas.at(i).id, updatedPolygon});
      }
    }
    recalculateAreasDataIsNeeded = false;
  }
  return solidAreas;
}

std::vector<Area>& ISceneObject::GetSimpleAreas() {

}

void ISceneObject::PositionSetX(float x) {
  position.setX(x);
  recalculateAreasDataIsNeeded = true;
}

void ISceneObject::PositionSetX(uint16_t x) {
  position.setX(x);
  recalculateAreasDataIsNeeded = true;
}

void ISceneObject::PositionSetY(float y) {
  position.setY(y);
  recalculateAreasDataIsNeeded = true;
}

void ISceneObject::PositionSetY(uint16_t y) {
  position.setY(y);
  recalculateAreasDataIsNeeded = true;
}

void ISceneObject::PositionAddX(float x) {
  position.addX(x);
  recalculateAreasDataIsNeeded = true;
}

void ISceneObject::PositionAddX(uint16_t x) {
  position.addX(x);
  recalculateAreasDataIsNeeded = true;
}

void ISceneObject::PositionAddY(float y) {
  position.addY(y);
  recalculateAreasDataIsNeeded = true;
}

void ISceneObject::PositionAddY(uint16_t y) {
  position.addY(y);
  recalculateAreasDataIsNeeded = true;
}

void ISceneObject::PositionSetOffset(int16_t x, int16_t y) {
  position.setOffset(x, y);
  recalculateAreasDataIsNeeded = true;
}

std::vector<uint16_t> ISceneObject::GetLowerBound() {
  std::vector<uint16_t> lowerBound{static_cast<uint16_t>(position.GetIntX() + boundingBox.lowerBoundX), static_cast<uint16_t>(position.GetIntY() + boundingBox.lowerBoundY)};
  return lowerBound;
}

std::vector<uint16_t> ISceneObject::GetUpperBound() {
  std::vector<uint16_t> upperBound{static_cast<uint16_t>(position.GetIntX() + boundingBox.upperBoundX), static_cast<uint16_t>(position.GetIntY() + boundingBox.upperBoundY)};
  return upperBound;
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
  return Update(KeyboardKeyCode::KEY_NONE);
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
