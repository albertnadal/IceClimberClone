#ifndef SIDE_WALL_H
#define SIDE_WALL_H

#include <iostream>
#include <vector>
#include <entity.h>
#include <state_machine.h>
#include <sprite.h>
#include <position.h>

using namespace std;

class SideWall: public IEntity
{
public:
  SideWall(EntityIdentificator, EntityType, SurfaceType, unsigned char, bool, bool);
  SideWall();
  ~SideWall();
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
  enum SideWallStateIdentificator {
      SIDE_WALL_MAX_STATES
  };
};

/* SideWallBlueLeft */

class SideWallBlueLeft : public SideWall
{
public:
  SideWallBlueLeft();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName();
  static IEntity* Create();
};

/* SideWallBlueRight */

class SideWallBlueRight : public SideWall
{
public:
  SideWallBlueRight();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName();
  static IEntity* Create();
};

/* SideWallBlueColumnsLeft */

class SideWallBlueColumnsLeft : public SideWall
{
public:
  SideWallBlueColumnsLeft();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName();
  static IEntity* Create();
};

/* SideWallBlueColumnsRight */

class SideWallBlueColumnsRight : public SideWall
{
public:
  SideWallBlueColumnsRight();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName();
  static IEntity* Create();
};

/* SideWallBrownLeft */

class SideWallBrownLeft : public SideWall
{
public:
  SideWallBrownLeft();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName();
  static IEntity* Create();
};

/* SideWallBrownRight */

class SideWallBrownRight : public SideWall
{
public:
  SideWallBrownRight();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName();
  static IEntity* Create();
};

/* SideWallBrownColumnsLeft */

class SideWallBrownColumnsLeft : public SideWall
{
public:
  SideWallBrownColumnsLeft();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName();
  static IEntity* Create();
};

/* SideWallBrownColumnsRight */

class SideWallBrownColumnsRight : public SideWall
{
public:
  SideWallBrownColumnsRight();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName();
  static IEntity* Create();
};

/* SideWallGreenLeft */

class SideWallGreenLeft : public SideWall
{
public:
  SideWallGreenLeft();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName();
  static IEntity* Create();
};

/* SideWallGreenRight */

class SideWallGreenRight : public SideWall
{
public:
  SideWallGreenRight();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName();
  static IEntity* Create();
};

/* SideWallGreenColumnsLeft */

class SideWallGreenColumnsLeft : public SideWall
{
public:
  SideWallGreenColumnsLeft();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName();
  static IEntity* Create();
};

/* SideWallGreenColumnsRight */

class SideWallGreenColumnsRight : public SideWall
{
public:
  SideWallGreenColumnsRight();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName();
  static IEntity* Create();
};

/* SideWallIceModelAUnbreakable */

class SideWallIceModelAUnbreakable : public SideWall
{
public:
  SideWallIceModelAUnbreakable();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName();
  static IEntity* Create();
};

/* SideWallIceModelBUnbreakable */

class SideWallIceModelBUnbreakable : public SideWall
{
public:
  SideWallIceModelBUnbreakable();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName();
  static IEntity* Create();
};

#endif
