#ifndef SIDE_WALL_H
#define SIDE_WALL_H

#include <iostream>
#include <vector>
#include <scene_object.h>
#include <state_machine.h>
#include <sprite.h>
#include <position.h>

using namespace std;

class SideWall: public ISceneObject
{
  std::vector<SpriteData>::iterator currentAnimationSpriteIterator;
  std::vector<SpriteData> currentAnimationSprites;
  bool firstSpriteOfCurrentAnimationIsLoaded = false;
  bool animationHasOnlyOneSprite = false;
  bool animationLoaded = false;
  SpriteData NextSpriteData();
  void LoadNextSprite();
protected:
  void LoadAnimationWithId(uint16_t);
public:
  SideWall(SceneObjectIdentificator, SceneObjectType, SurfaceType, unsigned char, bool);
  SideWall();
  ~SideWall();
  virtual void InitWithSpriteSheet(ObjectSpriteSheet*);
  uint16_t Width();
  uint16_t Height();
  virtual void PrintName();
  bool Update(uint8_t);
  static ISceneObject* Create();
  bool BeginAnimationLoopAgain();
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
  void InitWithSpriteSheet(ObjectSpriteSheet*);
  void PrintName();
  static ISceneObject* Create();
};

/* SideWallBlueRight */

class SideWallBlueRight : public SideWall
{
public:
  SideWallBlueRight();
  void InitWithSpriteSheet(ObjectSpriteSheet*);
  void PrintName();
  static ISceneObject* Create();
};

/* SideWallBlueColumnsLeft */

class SideWallBlueColumnsLeft : public SideWall
{
public:
  SideWallBlueColumnsLeft();
  void InitWithSpriteSheet(ObjectSpriteSheet*);
  void PrintName();
  static ISceneObject* Create();
};

/* SideWallBlueColumnsRight */

class SideWallBlueColumnsRight : public SideWall
{
public:
  SideWallBlueColumnsRight();
  void InitWithSpriteSheet(ObjectSpriteSheet*);
  void PrintName();
  static ISceneObject* Create();
};

/* SideWallBrownLeft */

class SideWallBrownLeft : public SideWall
{
public:
  SideWallBrownLeft();
  void InitWithSpriteSheet(ObjectSpriteSheet*);
  void PrintName();
  static ISceneObject* Create();
};

/* SideWallBrownRight */

class SideWallBrownRight : public SideWall
{
public:
  SideWallBrownRight();
  void InitWithSpriteSheet(ObjectSpriteSheet*);
  void PrintName();
  static ISceneObject* Create();
};

/* SideWallBrownColumnsLeft */

class SideWallBrownColumnsLeft : public SideWall
{
public:
  SideWallBrownColumnsLeft();
  void InitWithSpriteSheet(ObjectSpriteSheet*);
  void PrintName();
  static ISceneObject* Create();
};

/* SideWallBrownColumnsRight */

class SideWallBrownColumnsRight : public SideWall
{
public:
  SideWallBrownColumnsRight();
  void InitWithSpriteSheet(ObjectSpriteSheet*);
  void PrintName();
  static ISceneObject* Create();
};

/* SideWallGreenLeft */

class SideWallGreenLeft : public SideWall
{
public:
  SideWallGreenLeft();
  void InitWithSpriteSheet(ObjectSpriteSheet*);
  void PrintName();
  static ISceneObject* Create();
};

/* SideWallGreenRight */

class SideWallGreenRight : public SideWall
{
public:
  SideWallGreenRight();
  void InitWithSpriteSheet(ObjectSpriteSheet*);
  void PrintName();
  static ISceneObject* Create();
};

/* SideWallGreenColumnsLeft */

class SideWallGreenColumnsLeft : public SideWall
{
public:
  SideWallGreenColumnsLeft();
  void InitWithSpriteSheet(ObjectSpriteSheet*);
  void PrintName();
  static ISceneObject* Create();
};

/* SideWallGreenColumnsRight */

class SideWallGreenColumnsRight : public SideWall
{
public:
  SideWallGreenColumnsRight();
  void InitWithSpriteSheet(ObjectSpriteSheet*);
  void PrintName();
  static ISceneObject* Create();
};

#endif
