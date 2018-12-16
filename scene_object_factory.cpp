#include "scene_object_factory.h"
#include <map>
#include <string>

SceneObjectFactory::SceneObjectFactory(SceneObjectTextureManager* _textureManager) {
	textureManager = _textureManager;
	RegisterSceneObjects();
}

void SceneObjectFactory::RegisterSceneObjects() {
	std::cout << "REGISTERING OBJECTS." << std::endl;
	Register("MainCharacter", &MainCharacter::Create);
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
	if( it != m_FactoryMap.end() ) {
		ISceneObject *sceneObject = it->second();
		sceneObject->setTextureManager(textureManager);
		return sceneObject;
	}
	return NULL;
}

SceneObjectFactory *SceneObjectFactory::Get(SceneObjectTextureManager* _textureManager)
{
	static SceneObjectFactory instance(_textureManager);
	return &instance;
}
