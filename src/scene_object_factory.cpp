#include "scene_object_factory.h"
#include "object_sprite_sheet.h"
#include <map>

SceneObjectFactory::SceneObjectFactory(SceneObjectTextureManager* _textureManager) {
	textureManager = _textureManager;
	RegisterSceneObjects();
}

void SceneObjectFactory::RegisterSceneObjects() {
	std::cout << "REGISTERING OBJECTS." << std::endl;
	Register(SceneObjectIdentificator::MAIN_CHARACTER, &MainCharacter::Create);
	Register(SceneObjectIdentificator::BRICK, &Brick::Create);
	Register(SceneObjectIdentificator::BRICK_BROWN, &BrickBrown::Create);
	Register(SceneObjectIdentificator::BRICK_BLUE, &BrickBlue::Create);
	Register(SceneObjectIdentificator::BRICK_GREEN_HALF, &BrickGreenHalf::Create);
	Register(SceneObjectIdentificator::BRICK_BROWN_HALF, &BrickBrownHalf::Create);
	Register(SceneObjectIdentificator::BRICK_BLUE_HALF, &BrickBlueHalf::Create);
}

SceneObjectFactory &SceneObjectFactory::operator=(const SceneObjectFactory &) {
	return *this;
}

SceneObjectFactory::~SceneObjectFactory() {
	m_FactoryMap.clear();
}

void SceneObjectFactory::Register(const SceneObjectIdentificator sceneObjectId, CreateSceneObjectFn pfnCreate)
{
	m_FactoryMap[sceneObjectId] = pfnCreate;
}

ISceneObject *SceneObjectFactory::CreateSceneObject(const SceneObjectIdentificator sceneObjectId)
{
	FactoryMap::iterator it = m_FactoryMap.find(sceneObjectId);
	if( it != m_FactoryMap.end() ) {
		ISceneObject *sceneObject = it->second();
		ObjectSpriteSheet *objectSpriteSheet = textureManager->GetSpriteSheetBySceneObjectIdentificator(sceneObject->Id());
		sceneObject->InitWithSpriteSheet(objectSpriteSheet);
		return sceneObject;
	}
	return NULL;
}

SceneObjectFactory *SceneObjectFactory::Get(SceneObjectTextureManager* _textureManager)
{
	static SceneObjectFactory instance(_textureManager);
	return &instance;
}
