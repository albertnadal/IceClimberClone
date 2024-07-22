#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <chrono>
#include <position.h>
#include <defines.h>
#include <entity_sprite_sheet.h>
#include <sprite.h>
#include <state_machine.h>
#include <AABB/AABB.h>

using namespace std;

class IEntity;

struct Boundaries { int lowerBoundX, lowerBoundY, upperBoundX, upperBoundY; };
struct ObjectCollision { IEntity* object; int horizontalCorrection; int verticalCorrection; };

class IEntity : public StateMachine
{
private:
  std::vector<Boundaries> solidAreas;
  std::vector<Boundaries> simpleAreas;
protected:
  aabb::Tree<IEntity*> *spacePartitionObjectsTree = nullptr;
  std::vector<SpriteData> currentAnimationSprites;
  std::vector<SpriteData>::iterator currentAnimationSpriteIterator;
  EntitySpriteSheet *spriteSheet = nullptr;
  EntityType type;
  chrono::system_clock::time_point nextSpriteTime;
  bool animationLoaded = false;
  bool firstSpriteOfCurrentAnimationIsLoaded = false;
  bool animationHasOnlyOneSprite = false;
  bool recalculateAreasDataIsNeeded = true;
  void RemoveFromSpacePartitionObjectsTree();
  void LoadAnimationWithId(uint16_t);
  void LoadNextSprite();
  SpriteData NextSpriteData();
  bool ShouldBeginAnimationLoopAgain();
public:
  IEntity();
  IEntity(EntityIdentificator, EntityType, SurfaceType, unsigned char, bool, bool);
  EntityIdentificator id;
  SurfaceType surfaceType;
  Sprite currentSprite;
  Position position;
  Boundaries boundingBox;
  Boundaries solidBoundingBox;
  collision::vec2<int16_t> vectorDirection;
  uint32_t uniqueId;
  bool isBreakable = false;
  bool isTraversable = false;
  bool isMarkedToDelete = false;
  void SetSpacePartitionObjectsTree(aabb::Tree<IEntity*>*);
  //std::vector<Area>& GetSolidAreas(); DEPRECATED Now this is GetAbsoluteSolidBoundaries
  //std::vector<Area>& GetSimpleAreas(); DEPRECATED Now this is GetAbsoluteSimpleBoundaries
  void PositionSetOffset(int16_t x, int16_t y);
  void PositionSetXY(float, float);
  void PositionSetX(float);
  void PositionSetY(float);
  void PositionAddX(float);
  void PositionAddY(float);
  virtual std::vector<int> GetLowerBound();
  virtual std::vector<int> GetUpperBound();
  virtual std::vector<int> GetSolidLowerBound();
  virtual std::vector<int> GetSolidUpperBound();
  virtual Boundaries GetAbsoluteBoundaries();
  virtual Boundaries GetAbsoluteSolidBoundaries();
  virtual EntityIdentificator Id();
  virtual EntityType Type();
  virtual void InitWithSpriteSheet(EntitySpriteSheet*);
  virtual int Width();
  virtual int Height();
  virtual void PrintName();
  virtual void PrintBoundaries();
  virtual bool Update();
  virtual bool Update(const uint8_t);
  virtual bool Update(const uint8_t, aabb::Tree<IEntity*>&);
  virtual void UpdatePositionInSpacePartitionTree();
  virtual void Hit(bool);
  virtual bool IsCloud();
  virtual bool IsTopi();
};

typedef IEntity* (*CreateEntityFn)(void);

#endif
