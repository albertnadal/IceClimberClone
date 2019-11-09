#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include <iostream>
#include <chrono>
#include <position.h>
#include <defines.h>
#include <object_sprite_sheet.h>
#include <sprite.h>
#include <state_machine.h>
#include <AABB/AABB.h>

using namespace std;

struct Boundaries { uint16_t lowerBoundX, lowerBoundY, upperBoundX, upperBoundY; };

class ISceneObject : public StateMachine
{
private:
  std::vector<Area> solidAreas;
  std::vector<Area> simpleAreas;
protected:
  aabb::Tree<ISceneObject*> *spacePartitionObjectsTree = nullptr;
  ObjectSpriteSheet *spriteSheet = nullptr;
  SceneObjectIdentificator id;
  SceneObjectType type;
  chrono::system_clock::time_point nextSpriteTime;
  bool recalculateAreasDataIsNeeded = true;
public:
  ISceneObject();
  ISceneObject(SceneObjectIdentificator, SceneObjectType, unsigned char);
  Sprite currentSprite;
  Position position;
  Boundaries boundingBox;
  uint32_t uniqueId;
  void SetSpacePartitionObjectsTree(aabb::Tree<ISceneObject*>*);
  std::vector<Area>& GetSolidAreas();
  std::vector<Area>& GetSimpleAreas();
  void PositionSetOffset(int16_t x, int16_t y);
  void PositionSetXY(float, float);
  void PositionSetX(float);
  void PositionSetY(float);
  void PositionAddX(float);
  void PositionAddY(float);
  void RecoverPreviousPosition();
  virtual std::vector<uint16_t> GetLowerBound();
  virtual std::vector<uint16_t> GetUpperBound();
  virtual SceneObjectIdentificator Id();
  virtual SceneObjectType Type();
  virtual void InitWithSpriteSheet(ObjectSpriteSheet*);
  virtual uint16_t Width();
  virtual uint16_t Height();
  virtual void PrintName();
  virtual void PrintBoundaries();
  virtual bool Update();
  virtual bool Update(const uint8_t);
  virtual bool Update(const uint8_t, aabb::Tree<ISceneObject*>&);
};

typedef ISceneObject* (*CreateSceneObjectFn)(void);

#endif
