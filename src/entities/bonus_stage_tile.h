#ifndef BONUS_STAGE_TILE_H
#define BONUS_STAGE_TILE_H

#include <iostream>
#include <vector>
#include <entity.h>
#include <state_machine.h>
#include <sprite.h>
#include <position.h>

using namespace std;

class BonusStageTile: public IEntity
{
public:
  BonusStageTile(EntityIdentificator, EntityType, SurfaceType, unsigned char, bool, bool);
  BonusStageTile();
  ~BonusStageTile();
  virtual void InitWithSpriteSheet(EntitySpriteSheet*) override;
  virtual void PrintName() const override;
  bool Update(uint8_t) override;
  static IEntity* Create();

private:
  // state map to define state function order
  BEGIN_STATE_MAP
  END_STATE_MAP

  // state enumeration order must match the order of state
  // method entries in the state map
  enum BonusStageTileStateIdentificator {
      BONUS_STAGE_TILE_MAX_STATES
  };
};

/* BonusStageTileA */

class BonusStageTileA : public BonusStageTile
{
public:
  BonusStageTileA();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName() const;
  static IEntity* Create();
};

/* BonusStageTileB */

class BonusStageTileB : public BonusStageTile
{
public:
  BonusStageTileB();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName() const;
  static IEntity* Create();
};

/* BonusStageTileC */

class BonusStageTileC : public BonusStageTile
{
public:
  BonusStageTileC();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName() const;
  static IEntity* Create();
};

#endif
