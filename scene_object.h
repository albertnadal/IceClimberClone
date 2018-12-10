#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include <iostream>
#include <vec2.h>
#include <sprite.h>

using namespace std;

class ISceneObject
{
public:
  Sprite currentSprite;
  vec2 position;
  virtual uint16 getWidth();
  virtual uint16 getHeight();
  virtual void PrintName();
};

typedef ISceneObject* (*CreateSceneObjectFn)(void);

#endif
