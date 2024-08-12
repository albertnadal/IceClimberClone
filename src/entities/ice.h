#ifndef ICE_H
#define ICE_H

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


class Ice: public IEntity
{
  Direction direction;
  void GetSolidCollisions(std::vector<ObjectCollision>&, bool&, bool&);

  std::optional<EntityIdentificator> fillHoleEntityId;
  IEntity* currentUnderlyingObject = nullptr;

  // Ice action states
  bool hasBeenPushedByTopi = false;    // The ice block has been pushed by a Topi
  bool isBeingPushed = false;          // The ice block is pushed by a Topi

  // Ice action update functions
  void UpdateCollisions();
  void MoveTo(Direction, float);
  bool ReachedScreenEdge();

public:
  Ice();
  ~Ice() override;
  void InitWithSpriteSheet(EntitySpriteSheet*) override;
  void PrintName() override;
  bool Update(uint8_t) override;
  static IEntity* Create();

  // state machine triggers
  bool ShouldBeginAnimationLoopAgain();

private:
  // state machine state functions
  void STATE_Move_Right();
  void STATE_Move_Left();

  // state map to define state function order
  BEGIN_STATE_MAP
      STATE_MAP_ENTRY(&Ice::STATE_Move_Right)
      STATE_MAP_ENTRY(&Ice::STATE_Move_Left)
  END_STATE_MAP

  // state enumeration order must match the order of state
  // method entries in the state map
  enum IceStateIdentificator {
      STATE_MOVE_RIGHT = 0, // Initial state
      STATE_MOVE_LEFT,
      ICE_MAX_STATES
  };
};

#endif
