#ifndef MAIN_CHARACTER_H
#define MAIN_CHARACTER_H

#include <iostream>
#include <scene_object.h>
#include <vec2.h>
#include <sprite.h>

using namespace std;

class MainCharacter: public ISceneObject
{
public:
  MainCharacter();
  ~MainCharacter();
  void InitWithSpriteSheet(ObjectSpriteSheet*);
  uint16 GetWidth();
  uint16 GetHeight();
  void PrintName();
  void Update();
  static ISceneObject* Create();
};

#endif
