#ifndef NUMBER_H
#define NUMBER_H

#include <iostream>
#include <vector>
#include <entity.h>
#include <state_machine.h>
#include <sprite.h>
#include <position.h>

using namespace std;

class Number: public IEntity
{
public:
  Number(EntityIdentificator, EntityType, SurfaceType, unsigned char, bool, bool);
  Number();
  ~Number();
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
  enum NumberStateIdentificator {
      NUMBER_MAX_STATES
  };
};

/* One */

class One : public Number
{
public:
  One();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName();
  static IEntity* Create();
};

/* Two */

class Two : public Number
{
public:
  Two();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName();
  static IEntity* Create();
};

/* Three */

class Three : public Number
{
public:
  Three();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName();
  static IEntity* Create();
};

/* Four */

class Four : public Number
{
public:
  Four();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName();
  static IEntity* Create();
};

/* Five */

class Five : public Number
{
public:
  Five();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName();
  static IEntity* Create();
};

/* Six */

class Six : public Number
{
public:
  Six();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName();
  static IEntity* Create();
};

/* Seven */

class Seven : public Number
{
public:
  Seven();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName();
  static IEntity* Create();
};

/* Eight */

class Eight : public Number
{
public:
  Eight();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName();
  static IEntity* Create();
};

#endif
