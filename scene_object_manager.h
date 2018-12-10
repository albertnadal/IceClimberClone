#ifndef SCENE_OBJECT_MANAGER_H
#define SCENE_OBJECT_MANAGER_H

#include <vector>
#include "scene_object_factory.h"
#include "scene_object_texture_manager.h"

class SceneObjectManager
{
  std::vector<ISceneObject*> objects;
  SceneObjectTextureManager *textureManager;
public:
  SceneObjectManager(SceneObjectTextureManager*);
  ~SceneObjectManager();
  void Update();
};

#endif
