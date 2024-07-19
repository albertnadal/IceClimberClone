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
  Direction direction;
  bool headedToRight = true;
  void LoadNextSprite();
  bool TopiIsQuiet();
  void GetSolidCollisions(std::vector<ObjectCollision>&, bool&, bool&);
  void DisplaceIfUnderlyingSurfaceIsMobile();
  bool ReachedScreenEdge();
  void SetRandomWalkStartPosition();

  collision::vec2<int16_t> vectorDirection;
  std::optional<SurfaceType> underlyingObjectSurfaceType; // REVISIT: Provably this info is not necessary for Topi
  std::optional<EntityIdentificator> objectToCarryId;
  IEntity* prevUnderlyingCloud = nullptr;
  IEntity* currentUnderlyingCloud = nullptr;
  std::vector<IEntity*> objectsToIgnoreDuringFall;

  // Topi action states
  bool isWalking = false;          // Topi is walking on a floor
  bool isRunning = false;          // Topi is running
  bool isFalling = false;          // Topi is falling
  bool isDazed = false;            // Topi is dazed
  bool isGoingToPickUpIce = false; // Topi is running to pick up ice on the floor edge
  bool isOnMobileSurface = false;  // Topi underlying surface is mobile

  // TODO: Topi action update functions
  void UpdateCollisions();
  void MoveTo(Direction, float);
  void FallDueToSuspendedInTheAir();

public:
  Topi();
  ~Topi() override;
  void InitWithSpriteSheet(EntitySpriteSheet*) override;
  int Width() override;
  int Height() override;
  void PrintName() override;
  bool IsCloud() override;
  bool Update(uint8_t) override;
  static IEntity* Create();

  // state machine triggers
  void HoleDetectedWhenWalking();
  bool ShouldBeginAnimationLoopAgain();

private:
  // state machine state functions
  void STATE_Walk_Right();
  void STATE_Walk_Left();
  void STATE_Run_To_Pick_Up_Ice_Right();
  void STATE_Run_To_Pick_Up_Ice_Left();

  // state map to define state function order
  BEGIN_STATE_MAP
      STATE_MAP_ENTRY(&Topi::STATE_Walk_Right)
      STATE_MAP_ENTRY(&Topi::STATE_Walk_Left)
      STATE_MAP_ENTRY(&Topi::STATE_Run_To_Pick_Up_Ice_Right)
      STATE_MAP_ENTRY(&Topi::STATE_Run_To_Pick_Up_Ice_Left)
  END_STATE_MAP

  // state enumeration order must match the order of state
  // method entries in the state map
  enum TopiStateIdentificator {
      STATE_WALK_RIGHT = 0, // Initial state
      STATE_WALK_LEFT,
      STATE_RUN_TO_PICK_UP_ICE_RIGHT,
      STATE_RUN_TO_PICK_UP_ICE_LEFT,
      TOPI_MAX_STATES
  };
};

#endif
