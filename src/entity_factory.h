#ifndef ENTITY_FACTORY_H
#define ENTITY_FACTORY_H

#include <map>
#include <optional>
#include <AABB/AABB.h>
#include <entity.h>
#include <entity_manager.h>
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

class EntityManager;

class EntityFactory
{
private:
  EntityFactory(EntityManager*, EntityDataManager*, aabb::Tree<IEntity*>*);
  EntityFactory &operator=(const EntityFactory &);
  void RegisterEntities();
  typedef map<EntityIdentificator, CreateEntityFn> FactoryMap;
  FactoryMap m_FactoryMap;
  EntityManager *entityManager = nullptr;
  EntityDataManager *textureManager = nullptr;
  aabb::Tree<IEntity*> *spacePartitionObjectsTree = nullptr;
public:
	~EntityFactory();
	static EntityFactory *Get(EntityManager*, EntityDataManager*, aabb::Tree<IEntity*>*);
	void Register(const EntityIdentificator, CreateEntityFn);
	std::optional<IEntity*> CreateEntity(const EntityIdentificator);
};

#endif
