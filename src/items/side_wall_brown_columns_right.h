#ifndef SIDE_WALL_BROWN_COLUMNS_RIGHT_H
#define SIDE_WALL_BROWN_COLUMNS_RIGHT_H

#include <iostream>
#include <vector>
#include <scene_object.h>
#include <state_machine.h>
#include <vec2.h>
#include <sprite.h>
#include <position.h>
#include <items/side_wall.h>

using namespace std;

class SideWallBrownColumnsRight : public SideWall
{
public:
  SideWallBrownColumnsRight();
  ~SideWallBrownColumnsRight();
  void InitWithSpriteSheet(ObjectSpriteSheet*);
  void PrintName();
  static ISceneObject* Create();
};

#endif
