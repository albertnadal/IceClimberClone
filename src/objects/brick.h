#ifndef BRICK_H
#define BRICK_H

#include <iostream>
#include <vector>
#include <scene_object.h>
#include <state_machine.h>
#include <vec2.h>
#include <sprite.h>
#include <position.h>

using namespace std;

class Brick: public ISceneObject
{
  std::vector<SpriteData>::iterator currentAnimationSpriteIterator;
  std::vector<SpriteData> currentAnimationSprites;
  bool firstSpriteOfCurrentAnimationIsLoaded = false;
  bool animationHasOnlyOneSprite = false;
  bool animationLoaded = false;
  void ProcessPressedKeys(bool = true);
  void ProcessReleasedKeys();
  SpriteData NextSpriteData();
  void LoadNextSprite();
protected:
  void LoadAnimationWithId(uint16_t);
public:
  Brick(SceneObjectIdentificator, SceneObjectType, unsigned char);
  Brick();
  ~Brick();
  virtual void InitWithSpriteSheet(ObjectSpriteSheet*);
  uint16_t Width();
  uint16_t Height();
  virtual void PrintName();
  bool Update(uint8_t);
  static ISceneObject* Create();

  void ReceiveHammerImpact();
  bool BeginAnimationLoopAgain();

private:
  // state machine state functions
  void STATE_Sticky();
  void STATE_Falling();

  // state map to define state function order
  BEGIN_STATE_MAP
      STATE_MAP_ENTRY(&Brick::STATE_Sticky)
      STATE_MAP_ENTRY(&Brick::STATE_Falling)
  END_STATE_MAP

  // state enumeration order must match the order of state
  // method entries in the state map
  enum BrickStateIdentificator {
      STATE_STICKY = 0, // Initial state
      STATE_FALLING,
      BRICK_MAX_STATES
  };
};

/* BrickBlue */

class BrickBlue : public Brick
{
public:
  BrickBlue();
  ~BrickBlue();
  void InitWithSpriteSheet(ObjectSpriteSheet*);
  void PrintName();
  static ISceneObject* Create();
};

/* BrickBlueHalf */

class BrickBlueHalf : public Brick
{
public:
  BrickBlueHalf();
  ~BrickBlueHalf();
  void InitWithSpriteSheet(ObjectSpriteSheet*);
  void PrintName();
  static ISceneObject* Create();
};

/* BrickBrown */

class BrickBrown : public Brick
{
public:
  BrickBrown();
  ~BrickBrown();
  void InitWithSpriteSheet(ObjectSpriteSheet*);
  void PrintName();
  static ISceneObject* Create();
};

/* BrickBrownHalf */

class BrickBrownHalf : public Brick
{
public:
  BrickBrownHalf();
  ~BrickBrownHalf();
  void InitWithSpriteSheet(ObjectSpriteSheet*);
  void PrintName();
  static ISceneObject* Create();
};

/* BrickGreenHalf */

class BrickGreenHalf : public Brick
{
public:
  BrickGreenHalf();
  ~BrickGreenHalf();
  void InitWithSpriteSheet(ObjectSpriteSheet*);
  void PrintName();
  static ISceneObject* Create();
};

#endif
