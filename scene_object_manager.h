#ifndef SCENE_OBJECT_MANAGER_H
#define SCENE_OBJECT_MANAGER_H

#include "scene_object_factory.h"
#include "scene_object_texture_manager.h"

class SceneObjectManager
{
  SceneObjectTextureManager *textureManager;
public:
  SceneObjectManager(SceneObjectTextureManager*);
  ~SceneObjectManager();
};

#endif
