#include "scene_object_factory.h"
#include <map>
#include <string>

SceneObjectFactory::SceneObjectFactory() {
	std::cout << "REGISTERING OBJECTS." << std::endl;
	Register("MainCharacter", &MainCharacter::Create);
}

SceneObjectFactory::SceneObjectFactory(const SceneObjectFactory &) {

}

SceneObjectFactory &SceneObjectFactory::operator=(const SceneObjectFactory &) {
	return *this;
}

SceneObjectFactory::~SceneObjectFactory() {
	m_FactoryMap.clear();
}

void SceneObjectFactory::Register(const string &sceneObjectName, CreateSceneObjectFn pfnCreate)
{
	m_FactoryMap[sceneObjectName] = pfnCreate;
}
ISceneObject *SceneObjectFactory::CreateSceneObject(const string &sceneObjectName)
{
	FactoryMap::iterator it = m_FactoryMap.find(sceneObjectName);
	if( it != m_FactoryMap.end() )
		return it->second();
	return NULL;
}

SceneObjectFactory *SceneObjectFactory::Get()
{
	static SceneObjectFactory instance;
	return &instance;
}
