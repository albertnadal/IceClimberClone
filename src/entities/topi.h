#ifndef TOPI_H
#define TOPI_H

#include <iostream>
#include <vector>
#include <optional>
#include <entity.h>
#include <state_machine.h>
#include <sprite.h>
#include <collision/collision.h>
#include <AABB/AABB.h>
#include <algorithm>
#include <defines.h>

using namespace std;


class Topi: public IEntity
{
  bool headedToRight = true;
  void ProcessReleasedKeys();
  bool TopiIsQuiet();
  void UpdatePreviousDirection();
  void GetSolidCollisions(std::vector<ObjectCollision>&, bool&);
  void DisplaceTopiIfUnderlyingSurfaceIsMobile();
  void CorrectTopiPositionOnReachScreenEdge();

  collision::vec2<int16_t> vectorDirection;
  collision::vec2<int16_t> prevVectorDirection;
  std::optional<SurfaceType> underlyingObjectSurfaceType;
  IEntity* prevUnderlyingCloud = nullptr;
  IEntity* currentUnderlyingCloud = nullptr;
  std::vector<IEntity*> objectsToIgnoreDuringFall;

  // Topi action states
  bool isRunning = false;          // Topi is running on a floor
  bool isFalling = false;          // Topi is falling
  bool isOnMobileSurface = false;  // Topi underlying surface is mobile

  // TODO: Topi action update functions
  void UpdateCollisions();

public:
  Topi();
  ~Topi() override;
  void InitWithSpriteSheet(EntitySpriteSheet*) override;
  uint16_t Width() override;
  uint16_t Height() override;
  void PrintName() override;
  bool IsCloud() override;
  bool Update(uint8_t) override;
  static IEntity* Create();

  // state machine triggers
  bool ShouldBeginAnimationLoopAgain();

private:
  // TODO: state machine state functions

  // state map to define state function order
  BEGIN_STATE_MAP
  END_STATE_MAP

  // state enumeration order must match the order of state
  // method entries in the state map
  enum TopiStateIdentificator {
      TOPI_MAX_STATES
  };
};

#endif
