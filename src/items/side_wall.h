#ifndef SIDE_WALL_H
#define SIDE_WALL_H

#include <iostream>
#include <vector>
#include <scene_object.h>
#include <state_machine.h>
#include <vec2.h>
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
  SideWall(SceneObjectIdentificator, SceneObjectType, unsigned char);
  SideWall();
  ~SideWall();
  virtual void InitWithSpriteSheet(ObjectSpriteSheet*);
  uint16_t Width();
  uint16_t Height();
  virtual void PrintName();
  bool Update(uchar);
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

#endif
