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

class ISceneObject : public StateMachine
{
protected:
  ObjectSpriteSheet *spriteSheet = nullptr;
  SceneObjectIdentificator id;
  chrono::system_clock::time_point nextSpriteTime;
public:
  ISceneObject();
  ISceneObject(SceneObjectIdentificator, unsigned char);
  Sprite currentSprite;
  Position position;
  virtual SceneObjectIdentificator Id();
  virtual void InitWithSpriteSheet(ObjectSpriteSheet*);
  virtual uint16 Width();
  virtual uint16 Height();
  virtual void PrintName();
  virtual bool Update(uchar);
};

typedef ISceneObject* (*CreateSceneObjectFn)(void);

#endif
