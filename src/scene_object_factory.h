#ifndef SCENE_OBJECT_FACTORY_H
#define SCENE_OBJECT_FACTORY_H

#include <map>
#include <string>
#include "scene_object.h"
#include "main_character.h"
#include "brick.h"
#include "scene_object_texture_manager.h"

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
