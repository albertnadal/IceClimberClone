#ifndef SCENE_OBJECT_FACTORY_H
#define SCENE_OBJECT_FACTORY_H

#include <map>
#include <string>
#include "scene_object.h"
#include "main_character.h"
#include "scene_object_texture_manager.h"

class SceneObjectFactory
{
private:
  SceneObjectFactory(SceneObjectTextureManager*);
  SceneObjectFactory &operator=(const SceneObjectFactory &);
  void RegisterSceneObjects();
  typedef map<string, CreateSceneObjectFn> FactoryMap;
  FactoryMap m_FactoryMap;
  SceneObjectTextureManager *textureManager;

public:
	~SceneObjectFactory();
	static SceneObjectFactory *Get(SceneObjectTextureManager*);
	void Register(const string &sceneObjectName, CreateSceneObjectFn pfnCreate);
	ISceneObject *CreateSceneObject(const string &sceneObjectName);
};

#endif
