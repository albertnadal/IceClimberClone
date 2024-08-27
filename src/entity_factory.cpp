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
	Register(EntityIdentificator::BONUS_STAGE_TEXT, &BonusStageText::Create);
	Register(EntityIdentificator::BONUS_STAGE_TILE_A, &BonusStageTileA::Create);
	Register(EntityIdentificator::BONUS_STAGE_TILE_B, &BonusStageTileB::Create);
	Register(EntityIdentificator::BONUS_STAGE_TILE_C, &BonusStageTileC::Create);
	Register(EntityIdentificator::EGGPLANT, &Eggplant::Create);
	Register(EntityIdentificator::CARROT, &Carrot::Create);
	Register(EntityIdentificator::CABBAGE, &Cabbage::Create);
	Register(EntityIdentificator::ZUCCHINI, &Zucchini::Create);
	Register(EntityIdentificator::CORN, &Corn::Create);
	Register(EntityIdentificator::TURNIP, &Turnip::Create);
	Register(EntityIdentificator::PUMPKIN, &Pumpkin::Create);
	Register(EntityIdentificator::ROMAINE_LETTUCE, &RomaineLettuce::Create);
	Register(EntityIdentificator::POTATO, &Potato::Create);
	Register(EntityIdentificator::MUSHROOM, &Mushroom::Create);
	Register(EntityIdentificator::ONE, &One::Create);
	Register(EntityIdentificator::TWO, &Two::Create);
	Register(EntityIdentificator::THREE, &Three::Create);
	Register(EntityIdentificator::FOUR, &Four::Create);
	Register(EntityIdentificator::FIVE, &Five::Create);
	Register(EntityIdentificator::SIX, &Six::Create);
	Register(EntityIdentificator::SEVEN, &Seven::Create);
	Register(EntityIdentificator::EIGHT, &Eight::Create);
	Register(EntityIdentificator::NITPICKER, &Nitpicker::Create);
	Register(EntityIdentificator::CONDOR, &Condor::Create);
	Register(EntityIdentificator::GAME_OVER, &GameOver::Create);
}

EntityFactory &EntityFactory::operator=(const EntityFactory &) {
	return *this;
}

EntityFactory::~EntityFactory() {
	entityFactoryMap.clear();
}

void EntityFactory::Register(const EntityIdentificator sceneObjectId, const CreateEntityFn pfnCreate)
{
	entityFactoryMap[sceneObjectId] = pfnCreate;
}

std::optional<IEntity*> EntityFactory::CreateEntity(const EntityIdentificator sceneObjectId) const
{
	EntityFactoryMap::const_iterator it = entityFactoryMap.find(sceneObjectId);
	if( it != entityFactoryMap.end() ) {
		IEntity *entity = it->second();
		std::optional<EntitySpriteSheet *> entitySpriteSheet = textureManager->GetSpriteSheetByEntityIdentificator(entity->Id());
		assert(entitySpriteSheet != std::nullopt);
		entity->SetEntityManager(entityManager);
		entity->SetSpacePartitionObjectsTree(spacePartitionObjectsTree);
		entity->InitWithSpriteSheet(*entitySpriteSheet);
		return entity;
	}

	return std::nullopt;
}

EntityFactory *EntityFactory::Get(EntityManager* _entityManager, EntityDataManager* _textureManager, aabb::Tree<IEntity*>* _spacePartitionObjectsTree)
{
	static EntityFactory instance(_entityManager, _textureManager, _spacePartitionObjectsTree);
	return &instance;
}
