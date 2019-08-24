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

struct Boundaries { uint16 lowerBoundX, lowerBoundY, upperBoundX, upperBoundY; };

class ISceneObject : public StateMachine
{
private:
  std::vector<Area> solidAreas;
  std::vector<Area> simpleAreas;
protected:
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
  uint32 uniqueId;
  std::vector<Area>& GetSolidAreas();
  std::vector<Area>& GetSimpleAreas();
  void PositionSetX(float);
  void PositionSetX(uint16);
  void PositionSetY(float);
  void PositionSetY(uint16);
  void PositionAddX(float);
  void PositionAddX(uint16);
  void PositionAddY(float);
  void PositionAddY(uint16);
  virtual std::vector<uint16> GetLowerBound();
  virtual std::vector<uint16> GetUpperBound();
  virtual SceneObjectIdentificator Id();
  virtual SceneObjectType Type();
  virtual void InitWithSpriteSheet(ObjectSpriteSheet*);
  virtual uint16 Width();
  virtual uint16 Height();
  virtual void PrintName();
  virtual void PrintBoundaries();
  virtual bool Update();
  virtual bool Update(const uchar);
  virtual bool Update(const uchar, aabb::Tree<ISceneObject*>&);
};

typedef ISceneObject* (*CreateSceneObjectFn)(void);

#endif
