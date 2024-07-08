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

class ISceneObject;

struct Boundaries { int lowerBoundX, lowerBoundY, upperBoundX, upperBoundY; };
struct ObjectCollision { ISceneObject* object; int horizontalCorrection; int verticalCorrection; };

class ISceneObject : public StateMachine
{
private:
  std::vector<Boundaries> solidAreas;
  std::vector<Boundaries> simpleAreas;
protected:
  aabb::Tree<ISceneObject*> *spacePartitionObjectsTree = nullptr;
  ObjectSpriteSheet *spriteSheet = nullptr;
  SceneObjectIdentificator id;
  SceneObjectType type;
  chrono::system_clock::time_point nextSpriteTime;
  bool recalculateAreasDataIsNeeded = true;
  void RemoveFromSpacePartitionObjectsTree();
public:
  ISceneObject();
  ISceneObject(SceneObjectIdentificator, SceneObjectType, SurfaceType, unsigned char, bool);
  SurfaceType surfaceType;
  Sprite currentSprite;
  Position position;
  Boundaries boundingBox;
  Boundaries solidBoundingBox;
  uint32_t uniqueId;
  bool isBreakable = false;
  bool isMarkedToDelete = false;
  void SetSpacePartitionObjectsTree(aabb::Tree<ISceneObject*>*);
  //std::vector<Area>& GetSolidAreas(); DEPRECATED Now this is GetAbsoluteSolidBoundaries
  //std::vector<Area>& GetSimpleAreas(); DEPRECATED Now this is GetAbsoluteSimpleBoundaries
  void PositionSetOffset(int16_t x, int16_t y);
  void PositionSetXY(float, float);
  void PositionSetX(float);
  void PositionSetY(float);
  void PositionAddX(float);
  void PositionAddY(float);
  void RecoverPreviousPosition();
  virtual std::vector<int> GetLowerBound();
  virtual std::vector<int> GetUpperBound();
  virtual std::vector<int> GetSolidLowerBound();
  virtual std::vector<int> GetSolidUpperBound();
  virtual Boundaries GetAbsoluteBoundaries();
  virtual Boundaries GetAbsoluteSolidBoundaries();
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
  virtual void Hit(bool);
};

typedef ISceneObject* (*CreateSceneObjectFn)(void);

#endif
