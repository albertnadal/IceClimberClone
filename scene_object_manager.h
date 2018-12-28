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
  uint32 *vbo;
  uint32 *ubo;
public:
  SceneObjectManager(SceneObjectTextureManager*, uint16*, uint32*, float*, uint32*);
  ~SceneObjectManager();
  void Update();
};

#endif
