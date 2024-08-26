#ifndef CONDOR_H
#define CONDOR_H

#include <iostream>
#include <vector>
#include <optional>
#include <chrono>
#include <entity.h>
#include <entity_manager.h>
#include <state_machine.h>
#include <sprite.h>
#include <collision/collision.h>
#include <AABB/AABB.h>
#include <algorithm>
#include <defines.h>

using namespace std;


class Condor: public IEntity
{
  std::optional<Direction> direction = std::nullopt;
  bool ReachedScreenEdge() override;
  void StartFlight();

  // Condor action states
  bool isFlying = false;           // Condor is flying
  bool isHunted = false;           // Condor is hunted, this way the update function became inoperative

  // Condor action update functions
  void MoveTo(Direction, float);

public:
  Condor();
  ~Condor() override;
  void InitWithSpriteSheet(EntitySpriteSheet*) override;
  void PrintName() override;
  bool Update(uint8_t) override;
  void Hit(bool) override;
  static IEntity* Create();

private:

  // state map to define state function order
  BEGIN_STATE_MAP
  END_STATE_MAP

  // state enumeration order must match the order of state
  // method entries in the state map
  enum CondorStateIdentificator {
      CONDOR_MAX_STATES
  };
};

#endif
