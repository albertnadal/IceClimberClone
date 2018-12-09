#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include <vec2.h>
#include <sprite.h>


using namespace std;

class SceneObject
{
public:
  Sprite currentSprite;
  vec2 position;

  SceneObject();
  ~SceneObject();

  uint16 getWidth();
  uint16 getHeight();
};

#endif
