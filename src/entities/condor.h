#ifndef CONDOR_H
#define CONDOR_H

#include <iostream>
#include <vector>
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
  Direction direction;
  bool ReachedScreenEdge();

  // Condor action states
  bool isFlying = false;           // Condor is flying

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
