#include "scene_object_factory.h"
#include "object_sprite_sheet.h"
#include <map>

SceneObjectFactory::SceneObjectFactory(SceneObjectDataManager* _textureManager, aabb::Tree<ISceneObject*>* _spacePartitionObjectsTree) {
	textureManager = _textureManager;
	spacePartitionObjectsTree = _spacePartitionObjectsTree;
	RegisterSceneObjects();
}

void SceneObjectFactory::RegisterSceneObjects() {
	//std::cout << "REGISTERING OBJECTS." << std::endl;
	Register(SceneObjectIdentificator::MAIN_CHARACTER, &MainCharacter::Create);
	Register(SceneObjectIdentificator::BRICK, &Brick::Create);
	Register(SceneObjectIdentificator::BRICK_BROWN, &BrickBrown::Create);
	Register(SceneObjectIdentificator::BRICK_BLUE, &BrickBlue::Create);
	Register(SceneObjectIdentificator::BRICK_GREEN_HALF, &BrickGreenHalf::Create);
	Register(SceneObjectIdentificator::BRICK_BROWN_HALF, &BrickBrownHalf::Create);
	Register(SceneObjectIdentificator::BRICK_BLUE_HALF, &BrickBlueHalf::Create);
	Register(SceneObjectIdentificator::SIDE_WALL_GREEN_LEFT, &SideWallGreenLeft::Create);
	Register(SceneObjectIdentificator::SIDE_WALL_GREEN_RIGHT, &SideWallGreenRight::Create);
	Register(SceneObjectIdentificator::SIDE_WALL_GREEN_COLUMNS_LEFT, &SideWallGreenColumnsLeft::Create);
	Register(SceneObjectIdentificator::SIDE_WALL_GREEN_COLUMNS_RIGHT, &SideWallGreenColumnsRight::Create);
	Register(SceneObjectIdentificator::SIDE_WALL_BROWN_COLUMNS_LEFT, &SideWallBrownColumnsLeft::Create);
	Register(SceneObjectIdentificator::SIDE_WALL_BROWN_COLUMNS_RIGHT, &SideWallBrownColumnsRight::Create);
	Register(SceneObjectIdentificator::SIDE_WALL_BROWN_LEFT, &SideWallBrownLeft::Create);
	Register(SceneObjectIdentificator::SIDE_WALL_BROWN_RIGHT, &SideWallBrownRight::Create);
	Register(SceneObjectIdentificator::SIDE_WALL_BLUE_LEFT, &SideWallBlueLeft::Create);
	Register(SceneObjectIdentificator::SIDE_WALL_BLUE_RIGHT, &SideWallBlueRight::Create);
	Register(SceneObjectIdentificator::SIDE_WALL_BLUE_COLUMNS_LEFT, &SideWallBlueColumnsLeft::Create);
	Register(SceneObjectIdentificator::SIDE_WALL_BLUE_COLUMNS_RIGHT, &SideWallBlueColumnsRight::Create);
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
		sceneObject->SetSpacePartitionObjectsTree(spacePartitionObjectsTree);
		sceneObject->InitWithSpriteSheet(objectSpriteSheet);
		return sceneObject;
	}
	return NULL;
}

SceneObjectFactory *SceneObjectFactory::Get(SceneObjectDataManager* _textureManager, aabb::Tree<ISceneObject*>* _spacePartitionObjectsTree)
{
	static SceneObjectFactory instance(_textureManager, _spacePartitionObjectsTree);
	return &instance;
}
