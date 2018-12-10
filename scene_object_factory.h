#ifndef SCENE_OBJECT_FACTORY_H
#define SCENE_OBJECT_FACTORY_H

#include <map>
#include <string>
#include "scene_object.h"
#include "main_character.h"

class SceneObjectFactory
{
private:
  SceneObjectFactory();
  SceneObjectFactory(const SceneObjectFactory &);
  SceneObjectFactory &operator=(const SceneObjectFactory &);
  typedef map<string, CreateSceneObjectFn> FactoryMap;
  FactoryMap m_FactoryMap;

public:
	~SceneObjectFactory();
	static SceneObjectFactory *Get();
	void Register(const string &sceneObjectName, CreateSceneObjectFn pfnCreate);
	ISceneObject *CreateSceneObject(const string &sceneObjectName);
};

#endif
