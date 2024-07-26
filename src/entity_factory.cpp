#include <entity_factory.h>
#include <entity_sprite_sheet.h>
#include <map>

EntityFactory::EntityFactory(EntityManager* _entityManager, EntityDataManager* _textureManager, aabb::Tree<IEntity*>* _spacePartitionObjectsTree) {
	entityManager = _entityManager;
	textureManager = _textureManager;
	spacePartitionObjectsTree = _spacePartitionObjectsTree;
	RegisterEntities();
}

void EntityFactory::RegisterEntities() {
	Register(EntityIdentificator::POPO, &Player::Create);
	Register(EntityIdentificator::BRICK, &Brick::Create);
	Register(EntityIdentificator::BRICK_BROWN, &BrickBrown::Create);
	Register(EntityIdentificator::BRICK_BLUE, &BrickBlue::Create);
	Register(EntityIdentificator::BRICK_GREEN_UNBREAKABLE, &BrickGreenUnbreakable::Create);
	Register(EntityIdentificator::BRICK_BROWN_UNBREAKABLE, &BrickBrownUnbreakable::Create);
	Register(EntityIdentificator::BRICK_BLUE_UNBREAKABLE, &BrickBlueUnbreakable::Create);
	Register(EntityIdentificator::BRICK_GREEN_HALF, &BrickGreenHalf::Create);
	Register(EntityIdentificator::BRICK_BROWN_HALF, &BrickBrownHalf::Create);
	Register(EntityIdentificator::BRICK_BLUE_HALF, &BrickBlueHalf::Create);
	Register(EntityIdentificator::BRICK_BLUE_CONVEYOR_BELT_RIGHT, &BrickBlueConveyorBeltRight::Create);
	Register(EntityIdentificator::BRICK_BLUE_CONVEYOR_BELT_LEFT, &BrickBlueConveyorBeltLeft::Create);
	Register(EntityIdentificator::BRICK_BROWN_CONVEYOR_BELT_RIGHT, &BrickBrownConveyorBeltRight::Create);
	Register(EntityIdentificator::BRICK_BROWN_CONVEYOR_BELT_LEFT, &BrickBrownConveyorBeltLeft::Create);
	Register(EntityIdentificator::BRICK_GREEN_CONVEYOR_BELT_RIGHT, &BrickGreenConveyorBeltRight::Create);
	Register(EntityIdentificator::BRICK_GREEN_CONVEYOR_BELT_LEFT, &BrickGreenConveyorBeltLeft::Create);
	Register(EntityIdentificator::BRICK_BLUE_CONVEYOR_BELT_RIGHT_UNBREAKABLE, &BrickBlueConveyorBeltRightUnbreakable::Create);
	Register(EntityIdentificator::BRICK_BLUE_CONVEYOR_BELT_LEFT_UNBREAKABLE, &BrickBlueConveyorBeltLeftUnbreakable::Create);
	Register(EntityIdentificator::BRICK_BROWN_CONVEYOR_BELT_RIGHT_UNBREAKABLE, &BrickBrownConveyorBeltRightUnbreakable::Create);
	Register(EntityIdentificator::BRICK_BROWN_CONVEYOR_BELT_LEFT_UNBREAKABLE, &BrickBrownConveyorBeltLeftUnbreakable::Create);
	Register(EntityIdentificator::BRICK_GREEN_CONVEYOR_BELT_RIGHT_UNBREAKABLE, &BrickGreenConveyorBeltRightUnbreakable::Create);
	Register(EntityIdentificator::BRICK_GREEN_CONVEYOR_BELT_LEFT_UNBREAKABLE, &BrickGreenConveyorBeltLeftUnbreakable::Create);
	Register(EntityIdentificator::SIDE_WALL_GREEN_LEFT, &SideWallGreenLeft::Create);
	Register(EntityIdentificator::SIDE_WALL_GREEN_RIGHT, &SideWallGreenRight::Create);
	Register(EntityIdentificator::SIDE_WALL_GREEN_COLUMNS_LEFT, &SideWallGreenColumnsLeft::Create);
	Register(EntityIdentificator::SIDE_WALL_GREEN_COLUMNS_RIGHT, &SideWallGreenColumnsRight::Create);
	Register(EntityIdentificator::SIDE_WALL_BROWN_COLUMNS_LEFT, &SideWallBrownColumnsLeft::Create);
	Register(EntityIdentificator::SIDE_WALL_BROWN_COLUMNS_RIGHT, &SideWallBrownColumnsRight::Create);
	Register(EntityIdentificator::SIDE_WALL_BROWN_LEFT, &SideWallBrownLeft::Create);
	Register(EntityIdentificator::SIDE_WALL_BROWN_RIGHT, &SideWallBrownRight::Create);
	Register(EntityIdentificator::SIDE_WALL_BLUE_LEFT, &SideWallBlueLeft::Create);
	Register(EntityIdentificator::SIDE_WALL_BLUE_RIGHT, &SideWallBlueRight::Create);
	Register(EntityIdentificator::SIDE_WALL_BLUE_COLUMNS_LEFT, &SideWallBlueColumnsLeft::Create);
	Register(EntityIdentificator::SIDE_WALL_BLUE_COLUMNS_RIGHT, &SideWallBlueColumnsRight::Create);
	Register(EntityIdentificator::CLOUD_SMALL, &CloudSmall::Create);
	Register(EntityIdentificator::CLOUD_BIG, &CloudBig::Create);
	Register(EntityIdentificator::CLOUD_TINY, &CloudTiny::Create);
	Register(EntityIdentificator::SIDE_WALL_ICE_MODEL_A_UNBREAKABLE, &SideWallIceModelAUnbreakable::Create);
	Register(EntityIdentificator::SIDE_WALL_ICE_MODEL_B_UNBREAKABLE, &SideWallIceModelBUnbreakable::Create);
	Register(EntityIdentificator::TOPI, &Topi::Create);
	Register(EntityIdentificator::ICE, &Ice::Create);
	Register(EntityIdentificator::WATER, &Water::Create);
}

EntityFactory &EntityFactory::operator=(const EntityFactory &) {
	return *this;
}

EntityFactory::~EntityFactory() {
	m_FactoryMap.clear();
}

void EntityFactory::Register(const EntityIdentificator sceneObjectId, CreateEntityFn pfnCreate)
{
	m_FactoryMap[sceneObjectId] = pfnCreate;
}

std::optional<IEntity*> EntityFactory::CreateEntity(const EntityIdentificator sceneObjectId)
{
	FactoryMap::iterator it = m_FactoryMap.find(sceneObjectId);
	if( it != m_FactoryMap.end() ) {
		IEntity *sceneObject = it->second();
		std::optional<EntitySpriteSheet *> entitySpriteSheet = textureManager->GetSpriteSheetByEntityIdentificator(sceneObject->Id());
		assert(entitySpriteSheet != std::nullopt);
		sceneObject->SetEntityManager(entityManager);
		sceneObject->SetSpacePartitionObjectsTree(spacePartitionObjectsTree);
		sceneObject->InitWithSpriteSheet(*entitySpriteSheet);
		return sceneObject;
	}

	return std::nullopt;
}

EntityFactory *EntityFactory::Get(EntityManager* _entityManager, EntityDataManager* _textureManager, aabb::Tree<IEntity*>* _spacePartitionObjectsTree)
{
	static EntityFactory instance(_entityManager, _textureManager, _spacePartitionObjectsTree);
	return &instance;
}
