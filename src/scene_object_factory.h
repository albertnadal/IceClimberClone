#ifndef SCENE_OBJECT_FACTORY_H
#define SCENE_OBJECT_FACTORY_H

#include <map>
#include "scene_object.h"
#include "scene_object_texture_manager.h"
#include "items/main_character.h"
#include "items/brick.h"
#include "items/brick_brown.h"
#include "items/brick_blue.h"
#include "items/brick_green_half.h"
#include "items/brick_brown_half.h"
#include "items/brick_blue_half.h"
#include "items/side_wall_green_left.h"
#include "items/side_wall_green_right.h"

class SceneObjectFactory
{
private:
  SceneObjectFactory(SceneObjectTextureManager*);
  SceneObjectFactory &operator=(const SceneObjectFactory &);
  void RegisterSceneObjects();
  typedef map<SceneObjectIdentificator, CreateSceneObjectFn> FactoryMap;
  FactoryMap m_FactoryMap;
  SceneObjectTextureManager *textureManager = nullptr;
public:
	~SceneObjectFactory();
	static SceneObjectFactory *Get(SceneObjectTextureManager*);
	void Register(const SceneObjectIdentificator, CreateSceneObjectFn);
	ISceneObject *CreateSceneObject(const SceneObjectIdentificator);
};

#endif
