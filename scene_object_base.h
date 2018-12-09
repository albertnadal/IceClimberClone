#ifndef SCENE_OBJECT_BASE_H
#define SCENE_OBJECT_BASE_H

#include <iostream>
#include <vec2.h>
#include <sprite.h>

using namespace std;

class SceneObjectBase
{
public:
  Sprite currentSprite;
  vec2 position;

  SceneObjectBase();
  virtual ~SceneObjectBase();
  virtual uint16 getWidth();
  virtual uint16 getHeight();
  virtual void PrintName();
};

#endif
