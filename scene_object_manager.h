#ifndef SCENE_OBJECT_MANAGER_H
#define SCENE_OBJECT_MANAGER_H

#include <vector>
#include "scene_object_factory.h"
#include "scene_object_texture_manager.h"

class SceneObjectManager
{
  std::vector<ISceneObject*> objects;
  SceneObjectTextureManager *textureManager;
  uint16 *vertices;
  float *uvs;
  unsigned int *vbo;
  unsigned int *ubo;
public:
  SceneObjectManager(SceneObjectTextureManager*, uint16*, unsigned int*, float*, unsigned int*);
  ~SceneObjectManager();
  void Update();
};

#endif
