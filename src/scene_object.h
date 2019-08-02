#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include <iostream>
#include <chrono>
#include <position.h>
#include <defines.h>
#include <object_sprite_sheet.h>
#include <sprite.h>
#include <state_machine.h>

using namespace std;

struct Boundaries { uint16 lowerBoundX, lowerBoundY, upperBoundX, upperBoundY; };

class ISceneObject : public StateMachine
{
protected:
  ObjectSpriteSheet *spriteSheet = nullptr;
  SceneObjectIdentificator id;
  SceneObjectType type;
  chrono::system_clock::time_point nextSpriteTime;
  uint32 uniqueId;
public:
  ISceneObject();
  ISceneObject(SceneObjectIdentificator, SceneObjectType, unsigned char);
  Sprite currentSprite;
  Position position;
  Boundaries boundingBox;
  virtual SceneObjectIdentificator Id();
  virtual SceneObjectType Type();
  virtual void InitWithSpriteSheet(ObjectSpriteSheet*);
  virtual uint16 Width();
  virtual uint16 Height();
  virtual void PrintName();
  virtual bool Update(uchar);
};

typedef ISceneObject* (*CreateSceneObjectFn)(void);

#endif
