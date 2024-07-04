#ifndef BRICK_H
#define BRICK_H

#include <iostream>
#include <vector>
#include <scene_object.h>
#include <state_machine.h>
#include <sprite.h>
#include <position.h>
#include <defines.h>

using namespace std;

class Brick: public ISceneObject
{
  std::vector<SpriteData>::iterator currentAnimationSpriteIterator;
  std::vector<SpriteData> currentAnimationSprites;
  bool firstSpriteOfCurrentAnimationIsLoaded = false;
  bool animationHasOnlyOneSprite = false;
  bool animationLoaded = false;
  bool isPropelled = false;
  void ProcessPressedKeys(bool = true);
  void ProcessReleasedKeys();
  SpriteData NextSpriteData();
  void LoadNextSprite();
  void Propel(float vSpeed, float hSpeed);
  void UpdatePropel();
  void FinishPropel();

  // Propel trajectory data
  float hInitialPropelSpeed = 0.0f;
  float vInitialPropelSpeed = 0.0f;
  float tPropel = 0.0f;
  float hInitialPropelPosition = 0.0f;
  float vInitialPropelPosition = 0.0f;
  float previous_vOffset = 0.0f;

protected:
  void LoadAnimationWithId(uint16_t);
public:
  Brick(SceneObjectIdentificator, SceneObjectType, SurfaceType, unsigned char, bool);
  Brick();
  ~Brick();
  virtual void InitWithSpriteSheet(ObjectSpriteSheet*);
  uint16_t Width();
  uint16_t Height();
  virtual void PrintName();
  bool Update(uint8_t);
  void Hit(bool) override;
  static ISceneObject* Create();

  // state machine triggers
  void Break();
  bool BeginAnimationLoopAgain();

private:
  // state machine state functions
  virtual void STATE_Sticky();
  virtual void STATE_Falling();

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

private:
  void STATE_Sticky() override;
  void STATE_Falling() override;
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

private:
  void STATE_Sticky() override;
  void STATE_Falling() override;
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

private:
  void STATE_Sticky() override;
  void STATE_Falling() override;
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

private:
  void STATE_Sticky() override;
  void STATE_Falling() override;
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

private:
  void STATE_Sticky() override;
  void STATE_Falling() override;
};

#endif
