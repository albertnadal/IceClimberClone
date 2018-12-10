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
  uint16 getWidth();
  uint16 getHeight();
  void PrintName();

  static ISceneObject* Create();
};

#endif
