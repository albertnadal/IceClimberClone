#ifndef SCENE_OBJECT_MANAGER_H
#define SCENE_OBJECT_MANAGER_H

#include <vector>
#include "scene_object_factory.h"
#include "scene_object_texture_manager.h"
#include "uint16_double_buffer.h"
#include "float_double_buffer.h"

class SceneObjectManager
{
  std::vector<ISceneObject*> objects;
  SceneObjectTextureManager *textureManager;
  UInt16DoubleBuffer *verticesDoubleBuffer;
  FloatDoubleBuffer *uvsDoubleBuffer;
  uint32 maxObjects;
public:
  SceneObjectManager(SceneObjectTextureManager*, UInt16DoubleBuffer*, FloatDoubleBuffer*, uint32);
  ~SceneObjectManager();
  void Update(uchar);
};

#endif
