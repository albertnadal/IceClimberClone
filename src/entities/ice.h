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
  bool isBeingDestroyed = false;       // The ice block is currently being destroyed due to a hit received from the player

  // Ice action update functions
  void UpdateCollisions();
  bool ReachedScreenEdge() override;

public:
  Ice();
  ~Ice() override;
  void InitWithSpriteSheet(EntitySpriteSheet*) override;
  void PrintName() override;
  bool Update(uint8_t) override;
  void Hit(bool) override;
  bool ShouldBeginAnimationLoopAgain() override;
  static IEntity* Create();

private:

  // state map to define state function order
  BEGIN_STATE_MAP
  END_STATE_MAP

  // state enumeration order must match the order of state
  // method entries in the state map
  enum IceStateIdentificator {
      ICE_MAX_STATES = 0, // Initial state
  };
};

#endif
