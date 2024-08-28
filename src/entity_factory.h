#ifndef ENTITY_FACTORY_H
#define ENTITY_FACTORY_H

#include <map>
#include <optional>
#include <AABB/AABB.h>
#include <entity.h>
#include <game_manager.h>
#include <entity_data_manager.h>
#include <entities/player.h>
#include <entities/brick.h>
#include <entities/side_wall.h>
#include <entities/cloud.h>
#include <entities/topi.h>
#include <entities/ice.h>
#include <entities/water.h>
#include <entities/bonus_stage_text.h>
#include <entities/bonus_stage_tile.h>
#include <entities/vegetable.h>
#include <entities/number.h>
#include <entities/nitpicker.h>
#include <entities/condor.h>
#include <entities/game_over.h>

class GameManager;

class EntityFactory
{
private:
  EntityFactory(GameManager*, EntityDataManager*, aabb::Tree<IEntity*>*);
  EntityFactory &operator=(const EntityFactory &);
  void RegisterEntities();
  typedef std::map<EntityIdentificator, CreateEntityFn> EntityFactoryMap;
  EntityFactoryMap entityFactoryMap;
  GameManager *gameManager = nullptr;
  EntityDataManager *textureManager = nullptr;
  aabb::Tree<IEntity*> *spacePartitionObjectsTree = nullptr;
public:
	~EntityFactory();
	static EntityFactory *Get(GameManager*, EntityDataManager*, aabb::Tree<IEntity*>*);
	void Register(const EntityIdentificator, CreateEntityFn);
	std::optional<IEntity*> CreateEntity(const EntityIdentificator) const;
};

#endif
