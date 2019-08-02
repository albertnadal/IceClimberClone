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
  void LoadAnimationWithId(uint16);
public:
  Brick(SceneObjectIdentificator, SceneObjectType, unsigned char);
  Brick();
  ~Brick();
  virtual void InitWithSpriteSheet(ObjectSpriteSheet*);
  uint16 Width();
  uint16 Height();
  virtual void PrintName();
  bool Update(uchar);
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

#endif
