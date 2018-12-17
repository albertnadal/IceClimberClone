#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include <iostream>
#include <vec2.h>
#include <types.h>
#include <object_sprite_sheet.h>
#include <sprite.h>

using namespace std;

class ISceneObject
{
protected:
  ObjectSpriteSheet *spriteSheet = nullptr;
  SceneObjectIdentificator id_ = SceneObjectIdentificator::NONE;
public:
  Sprite currentSprite;
  vec2 position;
  virtual SceneObjectIdentificator Id();
  virtual void InitWithSpriteSheet(ObjectSpriteSheet*);
  virtual uint16 Width();
  virtual uint16 Height();
  virtual void PrintName();
  virtual void Update();
};

typedef ISceneObject* (*CreateSceneObjectFn)(void);

#endif
