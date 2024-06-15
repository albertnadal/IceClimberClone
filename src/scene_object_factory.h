#ifndef SCENE_OBJECT_FACTORY_H
#define SCENE_OBJECT_FACTORY_H

#include <map>
#include <AABB/AABB.h>
#include "scene_object.h"
#include "scene_object_data_manager.h"
#include "items/main_character.h"
#include "items/brick.h"
#include "items/side_wall.h"

class SceneObjectFactory
{
private:
  SceneObjectFactory(SceneObjectDataManager*, aabb::Tree<ISceneObject*>*);
  SceneObjectFactory &operator=(const SceneObjectFactory &);
  void RegisterSceneObjects();
  typedef map<SceneObjectIdentificator, CreateSceneObjectFn> FactoryMap;
  FactoryMap m_FactoryMap;
  SceneObjectDataManager *textureManager = nullptr;
  aabb::Tree<ISceneObject*> *spacePartitionObjectsTree = nullptr;
public:
	~SceneObjectFactory();
	static SceneObjectFactory *Get(SceneObjectDataManager*, aabb::Tree<ISceneObject*>*);
	void Register(const SceneObjectIdentificator, CreateSceneObjectFn);
	ISceneObject *CreateSceneObject(const SceneObjectIdentificator);
};

#endif
