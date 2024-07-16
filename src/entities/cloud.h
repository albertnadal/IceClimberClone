#ifndef CLOUD_H
#define CLOUD_H

#include <iostream>
#include <vector>
#include <entity.h>
#include <state_machine.h>
#include <sprite.h>
#include <position.h>
#include <defines.h>

using namespace std;

class Cloud: public IEntity
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
  Cloud(EntityIdentificator, EntityType, SurfaceType, unsigned char, bool, bool);
  Cloud();
  ~Cloud();
  virtual void InitWithSpriteSheet(EntitySpriteSheet*);
  uint16_t Width();
  uint16_t Height();
  virtual void PrintName();
  bool IsCloud() override;
  bool Update(uint8_t);
  static IEntity* Create();
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
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName();
  static IEntity* Create();
};

/* CloudBig */

class CloudBig : public Cloud
{
public:
  CloudBig();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName();
  static IEntity* Create();
};

/* CloudTiny */

class CloudTiny : public Cloud
{
public:
  CloudTiny();
  void InitWithSpriteSheet(EntitySpriteSheet*);
  void PrintName();
  static IEntity* Create();
};

#endif
