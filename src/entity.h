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
class EntityManager;

struct Boundaries { int lowerBoundX, lowerBoundY, upperBoundX, upperBoundY; };
struct ObjectCollision { IEntity* object; int horizontalCorrection; int verticalCorrection; };

class IEntity : public StateMachine
{
protected:
  EntityManager *entityManager = nullptr;
  aabb::Tree<IEntity*> *spacePartitionObjectsTree = nullptr;
  std::vector<SpriteData> currentAnimationSprites;
  std::vector<SpriteData>::iterator currentAnimationSpriteIterator;
  EntitySpriteSheet *spriteSheet = nullptr;
  chrono::system_clock::time_point nextSpriteTime;
  bool animationLoaded = false;
  bool firstSpriteOfCurrentAnimationIsLoaded = false;
  bool animationHasOnlyOneSprite = false;
  bool recalculateAreasDataIsNeeded = true;
  void RemoveFromSpacePartitionObjectsTree();
  void LoadAnimationWithId(uint16_t);
  void LoadNextSprite();
  SpriteData NextSpriteData();
  virtual bool ReachedScreenEdge() const;
public:
  IEntity();
  IEntity(EntityIdentificator, EntityType, SurfaceType, unsigned char, bool, bool);
  EntityIdentificator id;
  EntityType type;
  SurfaceType surfaceType;
  Sprite currentSprite;
  Position position;
  Boundaries boundingBox;
  Boundaries solidBoundingBox;
  std::optional<Boundaries> attackBoundingBox = std::nullopt;  // REVISIT: This attribute member must be moved to Player class, as the Player is the only object with attack areas
  collision::vec2<int16_t> vectorDirection;
  uint32_t uniqueId;
  bool isBreakable = false;
  bool isTraversable = false;
  bool isMarkedToDelete = false;
  void SetSpacePartitionObjectsTree(aabb::Tree<IEntity*>*);
  void SetEntityManager(EntityManager*);
  void PositionSetOffset(int16_t x, int16_t y);
  void PositionSetXY(float, float);
  void PositionSetX(float);
  void PositionSetY(float);
  void PositionAddX(float);
  void PositionAddY(float);
  virtual std::vector<int> GetLowerBound() const;
  virtual std::vector<int> GetUpperBound() const;
  virtual std::vector<int> GetSolidLowerBound() const;
  virtual std::vector<int> GetSolidUpperBound() const;
  virtual std::optional<std::vector<int>> GetAttackLowerBound() const;  // REVISIT: This method must be moved to Player class, as the Player is the only object with attack areas
  virtual std::optional<std::vector<int>> GetAttackUpperBound() const;  // REVISIT: This method must be moved to Player class, as the Player is the only object with attack areas
  virtual Boundaries GetAbsoluteBoundaries() const;
  virtual Boundaries GetAbsoluteSolidBoundaries() const;
  virtual std::optional<Boundaries> GetAbsoluteAttackBoundaries() const;  // REVISIT: This method must be moved to Player class, as the Player is the only object with attack areas
  virtual EntityIdentificator Id() const;
  virtual EntityType Type() const;
  virtual void InitWithSpriteSheet(EntitySpriteSheet*);
  virtual int Width() const;
  virtual int Height() const;
  virtual void PrintName() const;
  virtual bool Update();
  virtual bool Update(const uint8_t);
  virtual bool Update(const uint8_t, aabb::Tree<IEntity*>&);
  virtual void UpdatePositionInSpacePartitionTree();
  virtual void Hit(bool);
  virtual bool ShouldBeginAnimationLoopAgain();
};

typedef IEntity* (*CreateEntityFn)(void);

#endif
