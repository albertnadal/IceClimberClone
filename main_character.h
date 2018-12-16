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
  SceneObjectIdentificator Id = SceneObjectIdentificator::MAIN_CHARACTER;
  MainCharacter();
  ~MainCharacter();
  void InitWithTextureManager(SceneObjectTextureManager*);
  uint16 getWidth();
  uint16 getHeight();
  void PrintName();
  void Update();
  static ISceneObject* Create();
};

#endif
