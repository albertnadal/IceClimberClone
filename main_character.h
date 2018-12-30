#ifndef MAIN_CHARACTER_H
#define MAIN_CHARACTER_H

#include <iostream>
#include <vector>
#include <scene_object.h>
#include <vec2.h>
#include <sprite.h>

using namespace std;

class MainCharacter: public ISceneObject
{
  std::vector<SpriteData>::iterator currentAnimationSpriteIterator;
  std::vector<SpriteData> currentAnimationSprites;
  bool firstSpriteOfCurrentAnimationIsLoaded = false;
  bool animationHasOnlyOneSprite = false;
  bool animationLoaded = false;
  bool headedToRight = true;
  uchar prevPressedKeys = KEY_NONE;
  void ProcessPressedKeys(uchar);
  void LoadAnimationWithId(uint16);
  SpriteData NextSpriteData();
public:
  MainCharacter();
  ~MainCharacter();
  void InitWithSpriteSheet(ObjectSpriteSheet*);
  uint16 Width();
  uint16 Height();
  void PrintName();
  bool Update(uchar);
  static ISceneObject* Create();
};

#endif
