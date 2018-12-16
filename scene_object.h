#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include <iostream>
#include <vec2.h>
#include <types.h>
#include <scene_object_texture_manager.h>
#include <sprite.h>

using namespace std;

class ISceneObject
{
protected:
  SceneObjectTextureManager *textureManager = nullptr;
public:
  SceneObjectIdentificator Id;
  Sprite currentSprite;
  vec2 position;
  virtual void InitWithTextureManager(SceneObjectTextureManager*);
  virtual uint16 getWidth();
  virtual uint16 getHeight();
  virtual void PrintName();
  virtual void Update();
};

typedef ISceneObject* (*CreateSceneObjectFn)(void);

#endif
