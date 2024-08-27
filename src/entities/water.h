#ifndef WATER_H
#define WATER_H

#include <iostream>
#include <vector>
#include <entity.h>
#include <state_machine.h>
#include <sprite.h>
#include <position.h>

using namespace std;

class Water: public IEntity
{
public:
  Water(EntityIdentificator, EntityType, SurfaceType, unsigned char, bool, bool);
  Water();
  ~Water();
  virtual void InitWithSpriteSheet(EntitySpriteSheet*) override;
  virtual void PrintName() override;
  bool Update(uint8_t) override;
  static IEntity* Create();

private:
  // state map to define state function order
  BEGIN_STATE_MAP
  END_STATE_MAP

  // state enumeration order must match the order of state
  // method entries in the state map
  enum WaterStateIdentificator {
      WATER_MAX_STATES
  };
};

#endif
