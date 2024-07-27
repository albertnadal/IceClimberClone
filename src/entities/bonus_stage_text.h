#ifndef BONUS_STAGE_TEXT_H
#define BONUS_STAGE_TEXT_H

#include <iostream>
#include <vector>
#include <entity.h>
#include <state_machine.h>
#include <sprite.h>
#include <position.h>

using namespace std;

class BonusStageText: public IEntity
{
public:
  BonusStageText(EntityIdentificator, EntityType, SurfaceType, unsigned char, bool, bool);
  BonusStageText();
  ~BonusStageText();
  virtual void InitWithSpriteSheet(EntitySpriteSheet*);
  virtual void PrintName();
  bool Update(uint8_t);
  static IEntity* Create();

private:
  // state map to define state function order
  BEGIN_STATE_MAP
  END_STATE_MAP

  // state enumeration order must match the order of state
  // method entries in the state map
  enum BonusStageTextStateIdentificator {
      BONUS_STAGE_TEXT_MAX_STATES
  };
};

#endif
