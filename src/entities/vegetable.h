#ifndef VEGETABLE_H
#define VEGETABLE_H

#include <iostream>
#include <vector>
#include <entity.h>
#include <game_manager.h>
#include <state_machine.h>
#include <sprite.h>
#include <position.h>

using namespace std;

class Vegetable: public IEntity
{
public:
  Vegetable(EntityIdentificator, EntityType, SurfaceType, unsigned char, bool, bool);
  Vegetable();
  ~Vegetable();
  virtual void InitWithSpriteSheet(EntitySpriteSheet*) override;
  virtual void PrintName() const override;
  bool Update(uint8_t) override;
  void Hit(bool) override;
  static IEntity* Create();

private:
  // state map to define state function order
  BEGIN_STATE_MAP
  END_STATE_MAP

  // state enumeration order must match the order of state
  // method entries in the state map
  enum VegetableStateIdentificator {
      VEGETABLE_MAX_STATES
  };
};

/* Eggplant */

class Eggplant : public Vegetable
{
public:
  Eggplant();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName() const;
  static IEntity* Create();
};

/* Carrot */

class Carrot : public Vegetable
{
public:
  Carrot();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName() const;
  static IEntity* Create();
};

/* Cabbage */

class Cabbage : public Vegetable
{
public:
  Cabbage();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName() const;
  static IEntity* Create();
};

/* Zucchini */

class Zucchini : public Vegetable
{
public:
  Zucchini();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName() const;
  static IEntity* Create();
};

/* Corn */

class Corn : public Vegetable
{
public:
  Corn();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName() const;
  static IEntity* Create();
};

/* Turnip */

class Turnip : public Vegetable
{
public:
  Turnip();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName() const;
  static IEntity* Create();
};

/* Pumpkin */

class Pumpkin : public Vegetable
{
public:
  Pumpkin();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName() const;
  static IEntity* Create();
};

/* RomaineLettuce */

class RomaineLettuce : public Vegetable
{
public:
  RomaineLettuce();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName() const;
  static IEntity* Create();
};

/* Potato */

class Potato : public Vegetable
{
public:
  Potato();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName() const;
  static IEntity* Create();
};

/* Mushroom */

class Mushroom : public Vegetable
{
public:
  Mushroom();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName() const;
  static IEntity* Create();
};

#endif
