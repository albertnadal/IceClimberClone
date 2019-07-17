#ifndef BRICK_BROWN_H
#define BRICK_BROWN_H

#include <iostream>
#include <vector>
#include <scene_object.h>
#include <state_machine.h>
#include <vec2.h>
#include <sprite.h>
#include <position.h>
#include <items/brick.h>

using namespace std;

class BrickBrown : public Brick
{
public:
  BrickBrown();
  ~BrickBrown();
  void InitWithSpriteSheet(ObjectSpriteSheet*);
  void PrintName();
  static ISceneObject* Create();
};

#endif
