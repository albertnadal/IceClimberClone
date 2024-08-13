#ifndef TOPI_H
#define TOPI_H

#include <iostream>
#include <vector>
#include <optional>
#include <entity.h>
#include <entity_manager.h>
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
  void LoadNextSprite() ;
  void GetSolidCollisions(std::vector<ObjectCollision>&, bool&, bool&);
  bool ReachedScreenEdge();
  void SetRandomWalkStartPosition();

  std::optional<EntityIdentificator> objectToCarryId;
  IEntity* currentUnderlyingObject = nullptr;
  std::vector<IEntity*> objectsToIgnoreDuringFall;

  // Topi action states
  bool isWalking = false;          // Topi is walking on a floor
  bool isFalling = false;          // Topi is falling
  bool isDazed = false;            // Topi is dazed
  bool isGoingToPickUpIce = false; // Topi is running to pick up ice on the floor edge
  bool isGoingToRecover = false;   // Topi is running to recover due to a hammer hit or fall

  // Topi action update functions
  void UpdateCollisions();
  void MoveTo(Direction, float);
  void FinishFall();

public:
  Topi();
  ~Topi() override;
  void InitWithSpriteSheet(EntitySpriteSheet*) override;
  void PrintName() override;
  bool IsTopi() override;
  bool Update(uint8_t) override;
  void Hit(bool) override;
  bool ShouldBeginAnimationLoopAgain() override;
  static IEntity* Create();

  // state machine triggers
  void HoleDetectedWhenWalking();
  void SuspendedInTheAir();
  void FallLanding();
  void HitReceived();
  void ChangeDirection();
  void BringIceToFillHole();

private:
  // state machine state functions
  void STATE_Walk_Right();
  void STATE_Walk_Left();
  void STATE_Run_To_Pick_Up_Ice_Right();
  void STATE_Run_To_Pick_Up_Ice_Left();
  void STATE_Fall_Dazed_Right();
  void STATE_Fall_Dazed_Left();
  void STATE_Run_Dazed_Right();
  void STATE_Run_Dazed_Left();
  void STATE_Bring_Ice_Right();
  void STATE_Bring_Ice_Left();

  // state map to define state function order
  BEGIN_STATE_MAP
      STATE_MAP_ENTRY(&Topi::STATE_Walk_Right)
      STATE_MAP_ENTRY(&Topi::STATE_Walk_Left)
      STATE_MAP_ENTRY(&Topi::STATE_Run_To_Pick_Up_Ice_Right)
      STATE_MAP_ENTRY(&Topi::STATE_Run_To_Pick_Up_Ice_Left)
      STATE_MAP_ENTRY(&Topi::STATE_Fall_Dazed_Right)
      STATE_MAP_ENTRY(&Topi::STATE_Fall_Dazed_Left)
      STATE_MAP_ENTRY(&Topi::STATE_Run_Dazed_Right)
      STATE_MAP_ENTRY(&Topi::STATE_Run_Dazed_Left)
      STATE_MAP_ENTRY(&Topi::STATE_Bring_Ice_Right)
      STATE_MAP_ENTRY(&Topi::STATE_Bring_Ice_Left)
  END_STATE_MAP

  // state enumeration order must match the order of state
  // method entries in the state map
  enum TopiStateIdentificator {
      STATE_WALK_RIGHT = 0, // Initial state
      STATE_WALK_LEFT,
      STATE_RUN_TO_PICK_UP_ICE_RIGHT,
      STATE_RUN_TO_PICK_UP_ICE_LEFT,
      STATE_FALL_DAZED_RIGHT,
      STATE_FALL_DAZED_LEFT,
      STATE_RUN_DAZED_RIGHT,
      STATE_RUN_DAZED_LEFT,
      STATE_BRING_ICE_RIGHT,
      STATE_BRING_ICE_LEFT,
      TOPI_MAX_STATES
  };
};

#endif
