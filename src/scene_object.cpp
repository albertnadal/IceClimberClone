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

std::vector<Area>& ISceneObject::GetSolidAreas() {
  if(recalculateAreasDataIsNeeded) {
    std::cout << "RECALCULATING SOLID AREAS" << endl;
    // Updates the object solid areas values according to the current object position and current sprite areas
    solidAreas.clear();
    if(currentSprite.areas!=nullptr) {
      std::cout << "CURRENT SPRITE CONTAINS " << currentSprite.areas->solidAreas.size() << " SOLID AREAS" << endl;
      for(uint16 i=0; i<currentSprite.areas->solidAreas.size(); i++) {
        std::vector<collision::vec2<float>> updatedPoints;
        std::vector<collision::vec2<float>> &currentPoints = currentSprite.areas->solidAreas.at(i).polygon.vertices;
        for(uint16 j=0; j<currentPoints.size(); j++) {
          // Apply the current position to the current area point
          updatedPoints.push_back(collision::vec2<float>(currentPoints.at(j).x + position.x, currentPoints.at(j).y + position.y));
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

void ISceneObject::PositionSetX(uint16 x) {
  position.setX(x);
  recalculateAreasDataIsNeeded = true;
}

void ISceneObject::PositionSetY(float y) {
  position.setY(y);
  recalculateAreasDataIsNeeded = true;
}

void ISceneObject::PositionSetY(uint16 y) {
  position.setY(y);
  recalculateAreasDataIsNeeded = true;
}

void ISceneObject::PositionAddX(float x) {
  position.addX(x);
  recalculateAreasDataIsNeeded = true;
}

void ISceneObject::PositionAddX(uint16 x) {
  position.addX(x);
  recalculateAreasDataIsNeeded = true;
}

void ISceneObject::PositionAddY(float y) {
  position.addY(y);
  recalculateAreasDataIsNeeded = true;
}

void ISceneObject::PositionAddY(uint16 y) {
  position.addY(y);
  recalculateAreasDataIsNeeded = true;
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
