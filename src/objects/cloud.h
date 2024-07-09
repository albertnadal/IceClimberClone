#ifndef CLOUD_H
#define CLOUD_H

#include <iostream>
#include <vector>
#include <scene_object.h>
#include <state_machine.h>
#include <sprite.h>
#include <position.h>
#include <defines.h>

using namespace std;

class Cloud: public ISceneObject
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
  void UpdateFlight();
  bool flyToRight = true;
public:
  Cloud(SceneObjectIdentificator, SceneObjectType, SurfaceType, unsigned char, bool);
  Cloud();
  ~Cloud();
  virtual void InitWithSpriteSheet(ObjectSpriteSheet*);
  uint16_t Width();
  uint16_t Height();
  virtual void PrintName();
  bool IsCloud() override;
  bool Update(uint8_t);
  static ISceneObject* Create();
  bool BeginAnimationLoopAgain();

private:
  // state map to define state function order
  BEGIN_STATE_MAP
  END_STATE_MAP

  // state enumeration order must match the order of state
  // method entries in the state map
  enum CloudStateIdentificator {
      CLOUD_MAX_STATES
  };
};

/* CloudSmall */

class CloudSmall : public Cloud
{
public:
  CloudSmall();
  void InitWithSpriteSheet(ObjectSpriteSheet*);
  void PrintName();
  static ISceneObject* Create();
};

/* CloudBig */

class CloudBig : public Cloud
{
public:
  CloudBig();
  void InitWithSpriteSheet(ObjectSpriteSheet*);
  void PrintName();
  static ISceneObject* Create();
};

/* CloudTiny */

class CloudTiny : public Cloud
{
public:
  CloudTiny();
  void InitWithSpriteSheet(ObjectSpriteSheet*);
  void PrintName();
  static ISceneObject* Create();
};

#endif
